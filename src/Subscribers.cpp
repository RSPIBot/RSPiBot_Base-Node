#include "RSPiBot_Base-Node/Subscribers.h"

/**************************************************************************************\
                                 Subscribers
\**************************************************************************************/
ros::Subscriber sub_odom;						/**< subscriber for the odom signals  */

/**************************************************************************************\
                               	Vectors with the mailboxes
\**************************************************************************************/
std::vector< Mailbox<PositionData > > sub_v_positionMailbox;	
						/**< A vector containing all the mailboxes with Position data */

/**************************************************************************************\
                               	Threading variable
\**************************************************************************************/
//Thread sub_thread;					/**< The thread witch runs the subscribers        */
Mutex sub_positonMutex;				/**< Mutex for the sub_v_positionMailbox vector   */

/**************************************************************************************\
                               	General
\**************************************************************************************/


/**************************************************************************************\
                               	Request mailbox functions
\**************************************************************************************/

Mailbox<PositionData>* Sub_GetPositionDataMailbox()
{
	ROS_INFO("adding a subscriber to the Sub_GetPositionDataMailbox");
	Mailbox<PositionData>* out = NULL;
	Mailbox<PositionData> data(1);
	
	sub_positonMutex.Lock();
	
	sub_v_positionMailbox.push_back(data);
	out = &sub_v_positionMailbox.back();
	
	sub_positonMutex.Unlock();
	
	return out;
}


void Sub_Init(ros::NodeHandle* p_nh, int mailBoxVectorSize)
{
	sub_odom = p_nh->subscribe(sub_robotName+"/odom", 1000, Sub_OdomCallback);

	sub_v_positionMailbox.reserve(mailBoxVectorSize);
}
/**************************************************************************************\
                               	General functions
\**************************************************************************************/

/**************************************************************************************\
                               	CallBack functions
\**************************************************************************************/
void Sub_OdomCallback(const nav_msgs::Odometry::ConstPtr& odom)
{
	/* Calculate the current angle */
	tf::Pose pose;
	tf::poseMsgToTF(odom->pose.pose, pose);
	double angle = tf::getYaw(pose.getRotation());
	
	/* translate it from -pi <angle <pi to 0 < 2pi */
	if (angle < 0)
		angle += (2.0*PI);
	//angle += PI;
	angle *=  (180.0/PI);
	
	/* Fill up the fields */
	PositionData currentPosition;
	currentPosition.x = odom->pose.pose.position.x;
	currentPosition.y = odom->pose.pose.position.y;
	currentPosition.angle = angle;
	currentPosition.robotId = sub_robotNumber;
	
	/* put a new message in the mailbox */
	sub_positonMutex.Lock();
	for (int i = 0; i < sub_v_positionMailbox.size(); i++)
	{
		if (!sub_v_positionMailbox.at(i).IsEmpty())
			sub_v_positionMailbox.at(i).GetNewestMessageErase();
		sub_v_positionMailbox.at(i).PutMessage(currentPosition);
	}
	sub_positonMutex.Unlock();
	
}

