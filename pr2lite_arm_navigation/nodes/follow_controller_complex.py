#!/usr/bin/env python

"""
follow_controller.py - controller for a kinematic chain
Copyright (c) 2011 Vanadium Labs LLC. All right reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of Vanadium Labs LLC nor the names of its
contributors may be used to endorse or promote products derived
from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL VANADIUM LABS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""

# Pi Robot references:
# http://code.google.com/p/pi-robot-ros-pkg/source/browse/trunk/experimental/ros_by_example/pi_actions/nodes/head_track_node.py?r=558
# https://github.com/someh4x0r/prlite-pc/blob/master/pr2lite_arm_navigation/nodes/relax_all_servos.py

# upper arm tilt (linar actuators):
# min radians = 0.26799389
# max radians = 1.21295395
# radians / sec = .236240015 rads/sec
#
# AX-12 +- 2.618 radians

import roslib; roslib.load_manifest('pr2lite_arm_navigation')
import actionlib
import rospy
from sensor_msgs.msg import JointState
from control_msgs.msg import FollowJointTrajectoryAction
from diagnostic_msgs.msg import *
from std_msgs.msg import Float64
import rospy, time
from dynamixel_controllers.srv import TorqueEnable, SetSpeed
from collections import deque

class FollowController:
    """ A controller for joint chains, exposing a FollowJointTrajectory action.  """
    def __init__(self):

        name = rospy.get_name()
        rospy.logdebug("get_node name " + name )
        self.name = name
        rospy.logdebug("FollowController " + name)
        rospy.logdebug("init")

        # parameters: rates and joints
        self.rate = rospy.get_param('~controllers/'+ name +'/rate',50.0)

        self.joints = list()
        self.fudge_factor = list()
        # Get all parameter names
        parameters = rospy.get_param_names()
        #for parameter in parameters:
            # Look for the parameters that name the joints.
            #if parameter.find("joint_name") != -1:
              #self.joints.append(rospy.get_param(parameter))

        self.joints = rospy.get_param('~controllers/'+name+'/joints')

        self.controllers = list()
        self.fudge_factor = list()
        self.current_pos = list()
        self.current_pos_cnt = list()
        self.last_speed = list()

        # traj = goal.trajectory
        self.trajectory_goal = deque()
        self.cur_point = 0

        self.execute_joints = list()
        for joint in self.joints:
            #self.device.servos[joint].controller = self
            # Remove "_joint" from the end of the joint name to get the controller names.
            servo_nm = joint.split("_joint")[0]
            self.controllers.append(servo_nm + "_controller")
            self.current_pos.append(-10)
            self.current_pos_cnt.append(0)

        # action server for FollowController
        name = rospy.get_param('~controllers/'+name +'/action_name','follow_joint_trajectory')
        self.server = actionlib.SimpleActionServer(name, FollowJointTrajectoryAction, execute_cb=self.actionCb, auto_start=False)
        rospy.logdebug("Started FollowController ("+name+"). Joints: " + str (self.joints) + " Controllers: " + str(self.controllers))

        self.fudge_value = [rospy.get_param('~fudge_factor/' + joint + '/value', 0.0) for joint in self.joints]

        # /joint_states
        rospy.Subscriber('joint_states', JointState, self.getJointState)

        # Possible enhancement: subscribe to /joint_states
 
        # Start controller state subscribers
        self.position_pub = list()
        self.torque_services = list()
        self.speed_services = list()
        self.max_speed = 0.4
        self.max_speed_shoulder_pan = .15
          
        for c in self.controllers:
          if c != 'left_upper_arm_hinge_controller' and c != 'right_upper_arm_hinge_controller':
            c_srv = rospy.Publisher(c + '/command', Float64)
            self.position_pub.append(c_srv)
            rospy.logdebug("Real pos pub " + c)
            if c != 'left_shoulder_tilt_controller' and c != 'right_shoulder_tilt_controller':
            # c != 'left_upper_arm_hinge_joint' and c != 'right_upper_arm_hinge_joint':
              speed_service = c + '/set_speed'
              rospy.wait_for_service(speed_service)
              srv = rospy.ServiceProxy(speed_service, SetSpeed)
              self.speed_services.append(srv)
              if  c == 'left_shoulder_pan_controller' or c == 'right_shoulder_pan_controller':
                velocity = self.max_speed_shoulder_pan
              else:
                velocity = self.max_speed
              srv(velocity)
              self.last_speed.append(velocity)
              rospy.logdebug("Real speed " + c + " " + str(velocity))
            else:
              # add dummy service so positions align
              srv = self.speed_services[-1]
              self.speed_services.append(srv)
              self.last_speed.append(self.max_speed)
              rospy.logdebug("Dummy speed " + c)
          else:
            # add dummy services so positions align
            c_srv = self.position_pub[-1]
            self.position_pub.append(c_srv)
            srv = self.speed_services[-1]
            self.speed_services.append(srv)
            self.last_speed.append(self.max_speed)
            rospy.logdebug("Dummy speed " + c)
            rospy.logdebug("Dummy pos pub " + c)
          rospy.logdebug("Starting " + c + "/command")
          self.server.start()


    def actionCb(self, goal):
        rospy.logdebug(self.name + ": Action goal recieved.")
        traj = goal.trajectory

        if set(self.joints) != set(traj.joint_names):
            msg = "Trajectory joint names does not match action controlled joints." + str(traj.joint_names) + " versus" + str(self.joints)
            rospy.logerr(msg)
            self.server.set_aborted(text=msg)
            return

        if not traj.points:
            msg = "Trajectory empty."
            rospy.logerr(msg)
            self.server.set_aborted(text=msg)
            return

        try:
            indexes = [traj.joint_names.index(joint) for joint in self.joints]
        except ValueError as val:
            msg = "Trajectory invalid."
            rospy.logerr(msg)
            self.server.set_aborted(text=msg)
            return
        if len(self.trajectory_goal) == 0:
          # full set of joints to execute for the trajectory being added
          self.cur_point = 0
          # eligible trajectory for getJointState to exec
          self.trajectory_goal.append(goal)
          self.execute_positions = list(goal.trajectory.points[self.cur_point].positions)
          self.execute_joints = list(goal.trajectory.joint_names)
          rospy.loginfo('follow_controller: new goal')
        else:
          # add to end of list
          rospy.logdebug( self.trajectory_goal)
          rospy.logdebug( self.execute_joints)
          rospy.logdebug( self.execute_positions)
          self.trajectory_goal.append(goal)
          rospy.loginfo('follow_controller: appending goal')
        self.server.set_succeeded()

    def getJointState(self, msg):
        # phase 1: clean up joints that met their goal
        i = 0
        for joint in self.joints:
          j = 0
          for joint_state_name in msg.name:
            if joint == joint_state_name:
              tolerance = .01
              if self.current_pos[i] == msg.position[j]:
                # give it a second to get started
                tolerance = .01 * max(self.current_pos_cnt[i] - 4, 1)
                self.current_pos_cnt[i] += 1
              else:
                self.current_pos_cnt[i] = 0
              self.current_pos[i] = msg.position[j]
              # remove trajectories already completed
              for k in range(len(self.execute_joints)):
                if self.execute_joints[k] == joint_state_name:
                  # desired = self.execute_positions[k] + self.fudge_value[i]
                  # IndexError: list index out of range
                  rospy.logdebug("desired index k %d" % k)
                  desired = self.execute_positions[k] 
                  if abs(self.execute_positions[k] - msg.position[j]) < tolerance or joint == 'left_upper_arm_hinge_joint' or joint == 'right_upper_arm_hinge_joint' or joint == 'left_shoulder_tilt_joint' or joint == 'right_shoulder_tilt_joint':
                    # close enough to consider the goal met
                    if self.current_pos_cnt[i] - 5 > 0:
                      rospy.loginfo('Stall Warning: ' + joint + ' cur_pos:' + str (msg.position[j]) + " desired_pos " + str(self.execute_positions[k]))
                    del self.execute_positions[k]
                    del self.execute_joints[k]
                    rospy.logdebug("del index k %d" % k)
                    # del changes indexing, so return and process rest in nxt cb
                    return
                    # break
                  rospy.logdebug(joint + ' desired_pos:' + str(desired) + ' cur_pos:' + str (msg.position[j]))
            j += 1
          i += 1

        # phase 2: if at desired trajectory, do next trajectory or goal
        # if trajectory points are empty, then done
        if len(self.trajectory_goal) == 0:
          return
        if len(self.execute_positions) == 0:
          # if no more joints in current point, try next point.
          # IndexError: list assignment index out of range
          rospy.logdebug("del index cur_point %d" % self.cur_point)
          del self.trajectory_goal[0].trajectory.points[self.cur_point]
          self.cur_point = 0
          # if no more points, try new goal.
          if len(self.trajectory_goal[0].trajectory.points) == 0:
            self.trajectory_goal.popleft()
            self.cur_point = 0
            if len(self.trajectory_goal) == 0:
              return
          self.execute_joints = list(self.trajectory_goal[0].trajectory.joint_names)
          self.execute_positions = list(self.trajectory_goal[0].trajectory.points[self.cur_point].positions)

        # phase 3: execute goal positions
        start = rospy.Time.now()
        nowsecs = rospy.Time.now().to_sec()
        for i, exec_joint in enumerate(self.execute_joints):
          for j,jnt in enumerate(self.joints):
            if jnt == exec_joint:
              match = j
              break
          desired = self.execute_positions[i] + self.fudge_value[match]
          #desired = self.execute_positions[i] 
          endtime = start + self.trajectory_goal[0].trajectory.points[self.cur_point].time_from_start
          endsecs = endtime.to_sec()
          velocity = abs((desired-msg.position[match])/ (endsecs-nowsecs))

          if  exec_joint == 'left_shoulder_pan_joint' or exec_joint == 'right_shoulder_pan_joint':
            velocity = self.max_speed_shoulder_pan
          elif velocity > self.max_speed:
            velocity = self.max_speed
          if exec_joint != 'left_upper_arm_hinge_joint' and exec_joint != 'right_upper_arm_hinge_joint':
            if exec_joint != 'left_shoulder_tilt_joint' and exec_joint != 'right_shoulder_tilt_joint':
               if self.last_speed[match] != velocity:
                   # ARD: ALWAYS FAILS
                   # self.speed_services[match](velocity)
                   self.last_speed[match] = velocity
            self.position_pub[match].publish(desired)
            rospy.logdebug('Trajectory ' + str(match) + ' ' + exec_joint + ' ' + str(desired) + ' ' + str(self.fudge_value[match]) + ' ' + str(velocity))
        return

if __name__ == '__main__':
  rospy.loginfo("FollowController init_node " )
  rospy.init_node("follow_controller")
  server = FollowController()
  rospy.logdebug("spin")
  rospy.spin()
  rospy.logdebug("post spin")
