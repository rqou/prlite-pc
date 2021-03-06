/*
 * Copyright (c) 2011, Vanadium Labs LLC
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Vanadium Labs LLC nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL VANADIUM LABS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Michael Ferguson, Helen Oleynikova
 */

#include <ros/ros.h>
#include <tf/tf.h>
#include <std_msgs/Float64.h>
#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>

#include <turtlebot_block_manipulation/PickAndPlaceAction.h>
#include <actionlib/client/simple_action_client.h>

#include <geometry_msgs/PoseArray.h>
#include <arm_navigation_msgs/MoveArmAction.h>
#include <arm_navigation_msgs/utils.h>

#define EXECUTE_GOAL \
    if (nh_.ok()) \
    {\
      bool finished_within_time = false;\
      client_.sendGoal(goalA);\
      finished_within_time = client_.waitForResult(ros::Duration(200.0)); \
      if (!finished_within_time) \
      { \
        client_.cancelGoal(); \
        ROS_INFO("Timed out achieving goal A"); \
      } \
      else \
      { \
        actionlib::SimpleClientGoalState state = client_.getState(); \
        bool success = (state == actionlib::SimpleClientGoalState::SUCCEEDED); \
        if(success) \
          ROS_INFO("Action finished: %s",state.toString().c_str()); \
        else \
          ROS_INFO("Action failed: %s",state.toString().c_str()); \
      } \
    }

namespace turtlebot_block_manipulation
{

class PickAndPlaceServer
{
private:
   
  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<turtlebot_block_manipulation::PickAndPlaceAction> as_;
  std::string action_name_;
 
  turtlebot_block_manipulation::PickAndPlaceFeedback     feedback_;
  turtlebot_block_manipulation::PickAndPlaceResult       result_;
  turtlebot_block_manipulation::PickAndPlaceGoalConstPtr goal_;
 
 
  actionlib::SimpleActionClient<arm_navigation_msgs::MoveArmAction> client_;

  ros::Subscriber pick_and_place_sub_;
  ros::Publisher gripper;
 
  // Parameters from goal
  std::string arm_link;
  double gripper_open;
  double gripper_closed;
  double z_up;
public:
  PickAndPlaceServer(const std::string name) :
    nh_("~"), as_(name, false), action_name_(name), client_("/move_right_arm", true)
  {
 
    //register the goal and feeback callbacks
    as_.registerGoalCallback(boost::bind(&PickAndPlaceServer::goalCB, this));
    as_.registerPreemptCallback(boost::bind(&PickAndPlaceServer::preemptCB, this));
   
    as_.start();

    client_.waitForServer();
    ROS_INFO("Connected to server");


    gripper = nh_.advertise<std_msgs::Float64>("/parallel_gripper_controller/command", 1, false);
  }

  void goalCB()
  {
    ROS_INFO("[pick and place] Received goal!");
    goal_ = as_.acceptNewGoal();
    arm_link = goal_->frame;
    gripper_open = goal_->gripper_open;
    gripper_closed = goal_->gripper_closed;
    z_up = goal_->z_up;
   
    if (goal_->topic.length() < 1)
      pickAndPlace(goal_->pickup_pose, goal_->place_pose);
    else
      pick_and_place_sub_ = nh_.subscribe(goal_->topic, 1, &PickAndPlaceServer::sendGoalFromTopic, this);
  }

  void sendGoalFromTopic(const geometry_msgs::PoseArrayConstPtr& msg)
  {
    ROS_INFO("[pick and place] Got goal from topic! %s", goal_->topic.c_str());
    pickAndPlace(msg->poses[0], msg->poses[1]);
    pick_and_place_sub_.shutdown();
  }

  void preemptCB()
  {
    ROS_INFO("%s: Preempted", action_name_.c_str());
    // set the action state to preempted
    as_.setPreempted();
  }
 
  void pickAndPlace(const geometry_msgs::Pose& start_pose, const geometry_msgs::Pose& end_pose)
  {
    ROS_INFO("[pick and place] Picking. Also placing.");
 
    arm_navigation_msgs::MoveArmGoal goalA;
    std_msgs::Float64 grip_msg;
   
    /* open gripper */
    grip_msg.data = gripper_open;
    gripper.publish( grip_msg );
    ros::Duration(2.0).sleep(); 

    goalA.motion_plan_request.group_name = "/move_right_arm";
    goalA.motion_plan_request.num_planning_attempts = 1;
    goalA.motion_plan_request.allowed_planning_time = ros::Duration(5.0);

    nh_.param<std::string>("planner_id",goalA.motion_plan_request.planner_id,std::string(""));
    nh_.param<std::string>("planner_service_name",goalA.planner_service_name,std::string

("ompl_planning/plan_kinematic_path"));

    arm_navigation_msgs::SimplePoseConstraint desired_pose;
    desired_pose.header.frame_id = arm_link;
    desired_pose.link_name = "right_wrist_roll_link";

    /* arm straight up */
    btQuaternion temp;
    temp.setRPY(0,1.57,0);

    desired_pose.pose.position.x = start_pose.position.x;
    desired_pose.pose.position.y = start_pose.position.y;
    desired_pose.pose.position.z = z_up;

    desired_pose.pose.orientation.x = temp.getX();
    desired_pose.pose.orientation.y = temp.getY();
    desired_pose.pose.orientation.z = temp.getZ();
    desired_pose.pose.orientation.w = temp.getW();

    desired_pose.absolute_position_tolerance.x = 0.2;
    desired_pose.absolute_position_tolerance.y = 0.2;
    desired_pose.absolute_position_tolerance.z = 0.2;

    desired_pose.absolute_roll_tolerance = 0.4;
    desired_pose.absolute_pitch_tolerance = 0.4;
    desired_pose.absolute_yaw_tolerance = 0.4;
  
    arm_navigation_msgs::addGoalConstraintToMoveArmGoal(desired_pose,goalA);
    EXECUTE_GOAL;
    ros::Duration(2.0).sleep();

/* get_fk.py output picking up using right arm
 [header: 
  seq: 0
  stamp: 
    secs: 0
    nsecs: 0
  frame_id: top_shelf_link
pose: 
  position: 
    x: 0.666436563998
    y: -0.253894656661
    z: 0.30756118584
  orientation: 
    x: 0.998651674778
    y: -0.0519056675811
    z: -0.000795318077425
    w: 4.00672677996e-05]
*/

    /* go down */
    desired_pose.pose.position.z = start_pose.position.z;
    arm_navigation_msgs::addGoalConstraintToMoveArmGoal(desired_pose,goalA);
    EXECUTE_GOAL;
    ros::Duration(2.0).sleep();

    /* close gripper */
    grip_msg.data = gripper_closed;
    gripper.publish( grip_msg );
    ros::Duration(2.0).sleep();   

    /* go up */
    desired_pose.pose.position.z = z_up;
    arm_navigation_msgs::addGoalConstraintToMoveArmGoal(desired_pose,goalA);
    EXECUTE_GOAL;
    ros::Duration(2.0).sleep();

    /* hover over */
    desired_pose.pose.position.x = end_pose.position.x;
    desired_pose.pose.position.y = end_pose.position.y;
    desired_pose.pose.position.z = z_up;
    arm_navigation_msgs::addGoalConstraintToMoveArmGoal(desired_pose,goalA);
    EXECUTE_GOAL;
    ros::Duration(2.0).sleep();

    /* go down */
    desired_pose.pose.position.z = end_pose.position.z;
    arm_navigation_msgs::addGoalConstraintToMoveArmGoal(desired_pose,goalA);
    EXECUTE_GOAL;
    ros::Duration(2.0).sleep();

    /* open gripper */
    grip_msg.data = gripper_open;
    gripper.publish( grip_msg );
    ros::Duration(2.0).sleep();   

    /* go up */
    goalA.motion_plan_request.goal_constraints.position_constraints[0].position.z = z_up;
    EXECUTE_GOAL;
    ros::Duration(2.0).sleep();

    // goal.header.frame_id = arm_link;
  }
};

};


int main(int argc, char** argv)
{
  ros::init(argc, argv, "pick_and_place_action_server");

  turtlebot_block_manipulation::PickAndPlaceServer server("pick_and_place");
  ros::spin();

  return 0;
}
