#include "RSPiBot_Base-Node/Subscribers.h"

/**************************************************************************************\
                                 Subscribers
\**************************************************************************************/
ros::Subscriber sub_arduinoCommand;				/**< subscriber for the command signals  */

/**************************************************************************************\
                               	Vectors with the mailboxes
\**************************************************************************************/
std::vector< Mailbox<std::string > > sub_v_arduinoCommandMailbox;	
						/**< A vector containing all the mailboxes with Position data */

/**************************************************************************************\
                               	Threading variable
\**************************************************************************************/
//Thread sub_thread;					/**< The thread witch runs the subscribers        */
Mutex sub_arduinoCommandMutex;				/**< Mutex for the sub_v_positionMailbox vector   */

/**************************************************************************************\
                               	General
\**************************************************************************************/


/**************************************************************************************\
                               	Request mailbox functions
\**************************************************************************************/

Mailbox<std::string>* Sub_GetArduinoCommandMailbox()
{
	ROS_INFO("adding a subscriber to the Sub_GetPositionDataMailbox");
	Mailbox<std::string>* out = NULL;
	Mailbox<std::string> data;
	
	sub_arduinoCommandMutex.Lock();
	
	sub_v_arduinoCommandMailbox.push_back(data);
	out = &sub_v_arduinoCommandMailbox.back();
	
	sub_arduinoCommandMutex.Unlock();
	
	return out;
}


void Sub_Init(ros::NodeHandle* p_nh, int mailBoxVectorSize)
{
	sub_arduinoCommand = p_nh->subscribe("/aruino", 1000, Sub_ArduinoCommandCallback);

	sub_v_arduinoCommandMailbox.reserve(mailBoxVectorSize);
}
/**************************************************************************************\
                               	General functions
\**************************************************************************************/

/**************************************************************************************\
                               	CallBack functions
\**************************************************************************************/
void Sub_ArduinoCommandCallback(const std_msgs::String::ConstPtr& msg)
{
	std::string data = msg->data;
	
	sub_arduinoCommandMutex.Lock();
	for (int i = 0; i < sub_v_arduinoCommandMailbox.size(); i++)
	{
		sub_v_arduinoCommandMailbox.at(i).PutMessage(data);
	}
	sub_arduinoCommandMutex.Unlock();
	
}

