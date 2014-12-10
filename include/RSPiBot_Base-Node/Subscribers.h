#ifndef SUBSCRIBERS_H_
#define SUBSCRIBERS_H_

#include "../../../rs_multithreading/include_all.h"

/*ros includes */
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/LaserScan.h>
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
 * Get a mailbox for the ArduinoCommands
 *
 * @return a pointer to a mailbox with positionData
 */
Mailbox<std::string>* Sub_GetArduinoCommandMailbox();

/**
 * Initialise all the mailboxes
 *
 * @param p_nh a pointer to the nodehandle
 * @param mailBoxVectorSize make shure that this is set big enoth else there is a chanse that the pointers to the mailboxes get currepted
 */
void Sub_Init(ros::NodeHandle* p_nh, int mailBoxVectorSize);

/* callback functions */

/**
 * This function does some pre porssessing over the message data and puts the data in the mailboxes
 *
 * @param the message
 */
void Sub_ArduinoCommandCallback(const std_msgs::String::ConstPtr& msg);



#endif