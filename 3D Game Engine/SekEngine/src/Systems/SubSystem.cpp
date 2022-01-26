#include "SubSystem.h"

bool SubSystem::Debugging = false;
bool SubSystem::Use_Profiling = false;
bool SubSystem::level_editing;
bool SubSystem::running_sim;
bool SubSystem::show_fps = false;

Clock::time_point SubSystem::CurrentTime; //current time
Clock::time_point SubSystem::OldTime; //time of previous frame
Clock::duration SubSystem::DeltaTime; //change in time between frames
double SubSystem::DeltaTimeMS; //change in time between frames

Clock::time_point SubSystem::CurrentSubTime; //current time for Subsystems
Clock::time_point SubSystem::SubOldTime; //time of subsystem start
Clock::duration SubSystem::SubDeltaTime; //change in time between subsystems

Clock::time_point SubSystem::GameStartTime = Clock::now(); //time when the game started after loading assets.
Clock::duration SubSystem::Timer; //Timer that measures the players progress on the level

int SubSystem::FPS;
bool SubSystem::Second_passed;

//sf::WindowHandle SubSystem::handle;// = (sf::WindowHandle)HWND;
//sf::Window* SubSystem::window;//(handle);

SubSystem::SubSystem()
{
}

SubSystem::~SubSystem()
{
}

void SubSystem::Init_Subsystem()
{
}

void SubSystem::Update_Subsystem()
{
}

void SubSystem::Profiling()
{
	if (Use_Profiling && Second_passed) {
		SubOldTime = CurrentSubTime;
		CurrentSubTime = Clock::now();
		SubDeltaTime = CurrentSubTime - SubOldTime;
		std::cout << "=================================================================" << "\n";
		std::cout << "SubSystem: " << SubSystem_name << "\n";
		std::cout << "Time taken between subsystems in nano seconds: " << SubDeltaTime.count() << "\n";
		std::cout << "Time taken between subsystems in milliseconds: " << SubDeltaTime.count() * 0.000001 << "\n";
		std::cout << "Time taken between subsystems in seconds: " << SubDeltaTime.count() * 0.000000001 << "\n";
	}
}

void SubSystem::message_finished_loading()
{
	std::cout << "...finished loading" << "\n";

	OldTime = CurrentTime;
	CurrentTime = Clock::now();
	DeltaTime = CurrentTime - OldTime;
	std::cout << "=================================================================" << "\n";
	std::cout << "Time taken to initialise in nano seconds: " << DeltaTime.count() << "\n";
	std::cout << "Time taken to initialise in milliseconds: " << DeltaTime.count() * 0.000001 << "\n";
	std::cout << "Time taken to initialise in seconds: " << DeltaTime.count() * 0.000000001 << "\n";
	std::cout << "=================================================================" << "\n";

	GameStartTime = Clock::now();
}
