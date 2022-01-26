#pragma once
#include <iostream>
#include "EventHandler.h"
#include "Event.h"
#include "EventPress.h"
#include "Parsing.h"
#include "Svec.h"
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

class SubSystem
{
public:
	static bool Debugging;
	static bool Use_Profiling;
	static bool level_editing;
	static bool running_sim;
	static bool show_fps;
	SubSystem();
	~SubSystem();
	virtual void Init_Subsystem();
	virtual void Update_Subsystem() = 0;
	virtual void Process_events() = 0;
	//inherited from SubSystem, calculates profiling with differences every SubSystem
	virtual void Profiling();
	//inherited from SubSystem, call this function once the last thing has been loaded
	virtual void message_finished_loading();

	static Clock::time_point CurrentTime; //current time
	static Clock::time_point CurrentSubTime; //current time for subsystems

	static Clock::time_point OldTime; //time of previous frame
	static Clock::time_point SubOldTime; //time of subsystem start

	static Clock::duration DeltaTime; //change in time between frames
	static Clock::duration SubDeltaTime; //change in time between subsystems
	static double DeltaTimeMS; //change in time between frames in MilliSeconds

	static Clock::time_point GameStartTime; //time when the game started after loading assets.
	static Clock::duration Timer; //Timer that measures the players progress on the level

	static int FPS;
	static bool Second_passed;
protected:
	std::string SubSystem_name;

	//static sf::WindowHandle handle;// = (sf::WindowHandle)HWND;
	//static sf::Window* window;//(handle);
};