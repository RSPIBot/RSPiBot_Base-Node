#include "RSPiBot_Base-Node/Subscribers.h"
#include "RSPiBot_Base-Node/main.h"

#include <unistd.h>

const char* portName = "/dev/ttyUSB0";

char buffer[100];
int counter =0;
int out = 0;
int start = -1;
int stop = -1;

SerialPort port = SerialPort(portName, true);

struct PublisherInfo
{
	std::string name;
	int frequency;
	clock_t lastUpdate;
	ros::Publisher publisher;
};

std::vector<PublisherInfo> publishers;

void Publish()
{
	for (int i = start; i <= stop; i++)
	{
		if (buffer[i] == '/')
		{
			printf("The name is ");
			for (int j = i+1; j <= stop; j++)
			{
				if (buffer[j] == '=')
					break;
				printf("%c",buffer[j]);  

			}
		}
			 
		if (buffer[i] == '=')
		{
			printf(" and its value is ");
			for (int j = i+1; j <= stop; j++)
			{
				if (buffer[j] =='/')
					break;
				printf("%c",buffer[j]);  
			}
			printf("\r\n");
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
				Publish();
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
	reciveThread.Start();
	
	ros::Rate loop_rate(10);
	while (ros::ok());
	return 0;
}
