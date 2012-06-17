#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Bool.h"

#include <sstream>
#include <cstdlib>

#include "std_msgs/String.h"
#include "packets_485net/packet_485net_dgram.h"

const uint16_t TORSO_UP = 1000;
const uint16_t TORSO_MID= 500;
const uint16_t TORSO_DOWN = 0;
const uint16_t ARM_DOWN = 1000;
const uint16_t ARM_UP = 0;
const uint16_t LINACT_PRECISION = 15;

#define AX12_FEEDBACK 0

ros::Subscriber joint_state_sub;
ros::Publisher linact_pub;
ros::Subscriber linact_sub;
ros::Publisher leftArmPublisher;
ros::Publisher rightArmPublisher;

#define LINACT_TORSO        0
#define LINACT_RIGHT_ARM    1
#define LINACT_LEFT_ARM     2
// #define LINACT_WHEELS       3
#define NUM_LINACT          3
#define LINACT_TORSO_ID     0x0C 
#define LINACT_RIGHT_ARM_ID 0x0E 
#define LINACT_LEFT_ARM_ID  0x0F 
#define LINACT_WHEELS_ID    0xAA 

int  linact_id[NUM_LINACT] = {LINACT_TORSO_ID, LINACT_RIGHT_ARM_ID, LINACT_LEFT_ARM_ID};
bool linact_arrived[NUM_LINACT];
bool linact_new_goal[NUM_LINACT];
bool linact_goal_last[NUM_LINACT];
int  linact_goal[NUM_LINACT] = {TORSO_DOWN, ARM_DOWN, ARM_DOWN};

void LinactPublish(int this_linact)
{
	uint16_t itmp;

  if (linact_arrived[this_linact] 
     && linact_goal[this_linact] == linact_goal_last[this_linact])
  {
    // publish wheel commands
    ROS_INFO("same goal: linact %d at %d", this_linact, linact_goal[this_linact]);
  }
  else 
  {
    if (linact_goal[this_linact] != linact_goal_last[this_linact])
    {
      // publish linear actuator command
      packets_485net::packet_485net_dgram linact_cmd;
      linact_cmd.destination = linact_id[this_linact];
          linact_cmd.source = 0xF0;
          linact_cmd.sport = 7;
          linact_cmd.dport = 1;

          itmp = linact_goal[this_linact] > LINACT_PRECISION ? linact_goal[this_linact] - LINACT_PRECISION : 0;
          linact_cmd.data.push_back(itmp & 0xFF);
          linact_cmd.data.push_back((itmp >> 8) & 0xFF);
          itmp = linact_goal[this_linact] < (1023-LINACT_PRECISION) ? linact_goal[this_linact] + LINACT_PRECISION : 1023;
          linact_cmd.data.push_back(itmp & 0xFF);
          linact_cmd.data.push_back((itmp >> 8) & 0xFF);

      linact_pub.publish(linact_cmd);
      linact_goal_last[this_linact] = linact_goal[this_linact];
      linact_new_goal[this_linact] = true;
      linact_arrived[this_linact] = false;
      ROS_INFO("linact %d at %d", this_linact, linact_goal[this_linact]);
    }
  }
}

void LinactCallback(const packets_485net::packet_485net_dgram& linear_actuator_status)
{
    int this_linact = NUM_LINACT;
    int i;

    for (i = 0; i < NUM_LINACT; i++) { 
      if(linear_actuator_status.source == linact_id[i]) {
        this_linact = i;
        break;
      }
    }
    if (this_linact == NUM_LINACT) {
      // ROS_INFO("invalid source %d", linear_actuator_status.source);
      return;
    }
    if(!(linear_actuator_status.destination == 0xF0
       || linear_actuator_status.destination == 0x00))
	return;
    if(linear_actuator_status.dport != 7) {
      ROS_INFO("invalid dport");
      return;
    }
    if(linear_actuator_status.data.size() != 7) {
      ROS_INFO("invalid size");
      return;
    }
    linact_arrived[this_linact] = linear_actuator_status.data[4+2];
    if (linact_new_goal[this_linact]) 
      // if new linear actuator goal then linact_arrived may take a 
      // few frames to update
    {
      if (!linact_arrived[this_linact])
        linact_new_goal[this_linact] = false;
      else
        linact_arrived[this_linact] = false;
    }
    // hack for if linear actuator isn't working
    //linact_arrived[this_linact] = true; 
    LinactPublish(this_linact);
}


/*void LinactInit(void)
{
  ros::NodeHandle n;
  int i;

  linact_sub = n.subscribe("net_485net_incoming_dgram", 1000, LinactCallback);
  linact_pub = n.advertise<packets_485net::packet_485net_dgram>("net_485net_outgoing_dgram", 1000);
  for (i = 0 ; i < NUM_LINACT; i++) {
    linact_goal_last[i] = 10000;
    linact_arrived[i] = false; 
    linact_new_goal[i] = false; 
  }
}*/

void setTorsoGoal(int goal)
{
  static int direction = 0;

  if (0 == goal) return;
  linact_goal[LINACT_TORSO] -= goal*LINACT_PRECISION;
  if (linact_goal[LINACT_TORSO] > TORSO_UP)
    linact_goal[LINACT_TORSO] = TORSO_UP;
  else if (linact_goal[LINACT_TORSO] < TORSO_DOWN)
    linact_goal[LINACT_TORSO] = TORSO_DOWN;
  if (
     !(direction == goal 
     && !linact_arrived[LINACT_TORSO]
     && linact_new_goal[LINACT_TORSO]
     )) {
    LinactPublish(LINACT_TORSO);
    direction = goal;
  } else {
    ROS_INFO("skip torso ");
  }
  ROS_INFO_STREAM("Torso: " << linact_goal[LINACT_TORSO]);
}

void setShoulderGoal(int right_goal, int left_goal)
{
  static int direction[2] = {0, 0};

  if(0 == left_goal && 0 == right_goal) return;
  linact_goal[LINACT_RIGHT_ARM] -= right_goal*LINACT_PRECISION;
  linact_goal[LINACT_LEFT_ARM] -= left_goal*LINACT_PRECISION;
  if (linact_goal[LINACT_RIGHT_ARM] < ARM_UP)
    linact_goal[LINACT_RIGHT_ARM] = ARM_UP;
  else if (linact_goal[LINACT_RIGHT_ARM] > ARM_DOWN)
    linact_goal[LINACT_RIGHT_ARM] = ARM_DOWN;
  if (linact_goal[LINACT_LEFT_ARM] < ARM_UP)
    linact_goal[LINACT_LEFT_ARM] = ARM_UP;
  else if (linact_goal[LINACT_LEFT_ARM] > ARM_DOWN)
    linact_goal[LINACT_LEFT_ARM] = ARM_DOWN;
  if (
     !(direction[0] == right_goal && !linact_arrived[LINACT_RIGHT_ARM]
     && linact_new_goal[LINACT_RIGHT_ARM]
     )) {
    LinactPublish(LINACT_RIGHT_ARM);
    direction[0] = right_goal;
  } else {
    // ROS_INFO("skip right Shoulder ");
  }
  if (
     !(direction[1] == left_goal && !linact_arrived[LINACT_LEFT_ARM]
     && linact_new_goal[LINACT_LEFT_ARM]
     )) {
    LinactPublish(LINACT_LEFT_ARM);
    direction[1] = left_goal;
  } else {
    // ROS_INFO("skip Left Shoulder ");
  }
  // ROS_INFO_STREAM("Left Shoulder: " << linact_goal[LINACT_LEFT_ARM] 
  //    << "  Right Shoulder: " << linact_goal[LINACT_RIGHT_ARM]);
}

int main (int argc, char** argv)
{
    ros::init(argc, argv, "linact_test");
    ros::NodeHandle n;
    int i;

    linact_sub = n.subscribe("net_485net_incoming_dgram", 1000, LinactCallback);
    linact_pub = n.advertise<packets_485net::packet_485net_dgram>("net_485net_outgoing_dgram", 1000);
    for (i = 0 ; i < NUM_LINACT; i++) {
        linact_goal_last[i] = 10000;
        linact_arrived[i] = false; 
        linact_new_goal[i] = false; 
    }

    setShoulderGoal(10, 10);

    ros::spin();

    return 0;
}

