//Rely on manager
#include "Manager.h"
//#include <Engine\SoundManager.h>

//Enter program
int main(int argc, char** argv)
{
	//Create manager object and call sequential functions
	Manager manager;
	manager.init();
	
	//SoundManager::Init(Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(0, 0, -1));
	//SoundManager::AddSFX("LDTune", "Assets/LD_Tune.wav");
	//SoundManager::PlaySFX("LDTune", Vec3(0, 0, 0), Vec3(0, 0, 0), true, 1, 1);
	
	//Enter main loop
	while (manager.state != programState::Closing)
	{
		//Call frame functions chronologically
		manager.input();
		manager.logic();
		manager.clear();
		manager.draw();
		manager.late();
	}

	//Done
	manager.quit();
	return 0;
}