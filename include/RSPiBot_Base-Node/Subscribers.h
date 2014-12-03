#ifndef SUBSCRIBERS_H_
#define SUBSCRIBERS_H_

#include "../../../multithreading/include_all.h"
#include "DataTypes.h"

/*ros includes */
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/LaserScan.h>
#include <tf/transform_broadcaster.h>
#include "std_msgs/String.h"
#include "templateFunctions.h"

#include <cmath>  

/**************************************************************************************\
                                       Defines
\**************************************************************************************/
//#define DEBUGMODUS
#define PI 3.14159265359

/**************************************************************************************\
                               	Function prototypes
\**************************************************************************************/
/* Get mailbox functions */
/**
 * Get a mailbox for the positionData
 *
 * @return a pointer to a mailbox with positionData of size(1)
 */
Mailbox<PositionData>* Sub_GetPositionDataMailbox();
/**
 * Get a mailbox for the LaserData
 *
 * @return a pointer to a mailbox with positionData of size(1)
 */
Mailbox<LaserData>* Sub_GetLaserDataMailbox();
/**
 * Get a mailbox for the robotList
 *
 * @return a pointer to a mailbox with positionData of size(1)
 */
Mailbox<RobotList>* Sub_GetRobotListMailbox();

/**
 * Initialise all the mailboxes
 *
 * @param p_nh a pointer to the nodehandle
 * @param mailBoxVectorSize make shure that this is set big enoth else there is a chanse that the pointers to the mailboxes get currepted
 */
void Sub_Init(ros::NodeHandle* p_nh, int mailBoxVectorSize);

/* general functions */
void Sub_SetRobotNumber(int number);

/* callback functions */

/**
 * This function does some pre porssessing over the odom data and puts the data in the mailboxes
 *
 * @param odom the odom data
 */
void Sub_OdomCallback(const nav_msgs::Odometry::ConstPtr& odom);

/**
 * This function does some pre porssessing over the laser data and puts the data in the mailboxes
 *
 * @param laser the laser data
 */
void Sub_LaserCallback(const sensor_msgs::LaserScan::ConstPtr& laser);

/**
 * This function does some pre porssessing over the string data and puts the data in the mailboxes
 *
 * @param msg the robotlist
 */
void Sub_RobotListCallback(const std_msgs::String::ConstPtr& msg);

#endif