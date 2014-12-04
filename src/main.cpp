#include "RSPiBot_Base-Node/Subscribers.h"
#include "RSPiBot_Base-Node/main.h"

#include <unistd.h>

const char* portName = "/dev/ttyUSB0";

char buffer[100];
int counter =0;
int out = 0;
int start = -1;
int stop = -1;

SerialPort port = SerialPort(portName, false);

struct PublisherInfo
{
	std::string name;
	ros::Publisher publisher;
};

std::vector<PublisherInfo> publishers;

void Publish(std::string name, std::string data, void* n)
{
	ros::NodeHandle *nh = (ros::NodeHandle*)n;
	bool found = false;
	std_msgs::String msg;
	msg.data = data;
	
	//printf("%s, %s \r\n",name.c_str(),data.c_str());
	
	for (int i = 0; i < publishers.size(); i++)
		if (publishers.at(i).name == name)
		{
			publishers.at(i).publisher.publish(msg);
			found = true;
			break;
		}
	if(!found)
	{
		PublisherInfo pubInfo;
		pubInfo.name = name;
		std::string pubname = "/rspibotarduino/"+name;
		pubInfo.publisher = nh->advertise<std_msgs::String>(pubname.c_str(), 1000);
		pubInfo.publisher.publish(msg);
		publishers.push_back(pubInfo);
	}
}

void ExtractData (void* n)
{
	std::string name = "";
	std::string data = "";
	for (int i = start; i <= stop; i++)
	{

		if (buffer[i] == '/')
		{
			//printf("The name is ");
			for (int j = i+1; j <= stop; j++)
			{
				if (buffer[j] == '=')
					break;
				name +=buffer[j];
				//printf("%c",buffer[j]);  
			}
			//printf("%s",name.c_str());
		}
			 
		if (buffer[i] == '=')
		{
			//printf(" and its value is ");
			for (int j = i+1; j <= stop; j++)
			{
				if (buffer[j] =='/')
					break;
				data += buffer[j];
				//printf("%c",buffer[j]);  
			}
			//printf("\r\n");
			
		}
		
		if (name != "" && data != "")
		{
			//printf("%s, %s \r\n",name.c_str(),data.c_str());
			Publish(name, data, n);	
			name = "";
			data = "";
		} 
	}
}
int ReceiveThread(void* ptr)
{
	int counter =0;
	while (counter <99)
	{
		char temp =port.ReadByte();
		if (temp != -1)
		{
			buffer[counter] = temp;
			buffer[counter+1] = '\0';
			if(counter > 0 && buffer[counter] == 'B' && buffer[counter-1] == 'F')
				start = counter +1;
			if(start > 0 && buffer[counter] == 'B' && buffer[counter-1] == 'L')
				stop = counter - 2;
			counter++;
			if (start > 0 && stop > start)
			{
				ExtractData(ptr);
				counter = 0;
				start = -1;
				stop = -1;
			}
		}
		else
		{
			usleep(100);
		}
		
	}
	return 0;
}
int SendThread(void* ptr)
{
	usleep(100);
	Mailbox<std::string>* commandMail = Sub_GetArduinoCommandMailbox();
	while(true)
	{
		if(!commandMail->IsEmpty())
		{
			std::string stringMessage = commandMail->GetMessage();
			printf("sending message %s", stringMessage.c_str());
			const char* message = "DR150FS";//stringMessage.c_str();
			port.Write((void*)message, sizeof(message));
		}
		else
		{
			usleep(100);
		}
	}
			
	return 0;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "RSPiBot_Base_Node", ros::init_options::AnonymousName);
	
	ros::NodeHandle n;
	
	Sub_Init(&n, 10);
	
	ros::AsyncSpinner spinner(0);
	spinner.start();
	
	usleep(1000 * 1000); // wait for a second to make shure evryting is started
	
	Thread reciveThread;
	reciveThread.SetThreadFunction(&ReceiveThread);
	reciveThread.SetParam(&n);
	reciveThread.Start();
	
	Thread sendThread;
	sendThread.SetThreadFunction(&SendThread);
	sendThread.SetParam(&n);
	sendThread.Start();
	
	ros::Rate loop_rate(10);
	while (ros::ok());
	return 0;
}
