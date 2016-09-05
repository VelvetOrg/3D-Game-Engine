//Rely on manager
#include "Manager.h"

//Enter program
int main(int argc, char** argv)
{
	//Create manager object and call sequential functions
	Manager manager;
	manager.init();

	//Enter main loop
	while (manager.state != programState::Closing)
	{
		//Call frame functions chronologically
		manager.early();
		manager.input();
		manager.logic();
		manager.draw();
		manager.late();
	}

	//Done
	manager.quit();
	return 0;
}