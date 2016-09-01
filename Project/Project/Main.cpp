//Rely on manager
#include "Manager.h"
#include <Engine\SoundManager.h>

//Enter program
int main(int argc, char** argv)
{
	//Create manager object and call sequential functions
	Manager manager;
	manager.init();
	unsigned int soundIDTAG;
	SoundManager::LoadAudio("Bomb.ogg", &soundIDTAG, true);
	SoundManager::PlayAudio(soundIDTAG, true);
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