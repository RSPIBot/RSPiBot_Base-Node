#include "RSPiBot_Base-Node/Subscribers.h"

int main(int argc, char **argv)
{
	int robotNumber = atoi(argv[1]);
	ros::init(argc, argv, "RSPiBot_Base-Node", ros::init_options::AnonymousName);
	ros::NodeHandle n;


	
	Sub_Init(&n, 10);
	ros::AsyncSpinner spinner(0);
	spinner.start();
	
	usleep(1000 * 1000); // wait for a second to make shure evryting is started
	


	
	int count = 0;
	while (ros::ok())
	{

	/* todo adde the send and recive stuff here */
	}
	
	
	return 0;
}
	