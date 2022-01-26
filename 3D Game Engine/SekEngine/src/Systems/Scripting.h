#pragma once
#include "vector"
#include "iostream"
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

/*
A class to be inherited by player built scripts, scripts should use polymorphism on the update function for the scripts to be executed each frome,
when a new script is added it should create an initialsie function, inside it contain NewScript(); and call their function in "IncludeAllScripts.h"
A single instance of Scripting should be present in GameEngine
*/
class Scripting
{
public:
	Scripting();
	~Scripting();
	//using polymorphism when this is called it will call its children update functions
	virtual void Update();
	//using polymorphism when this is called it will call its children destroy functions
	virtual void Destroy();
	//delete the scripts instacnes to rlease memory
	void delete_scripts();
	void show_profiling(bool state);
protected:
	static std::vector<Scripting*> Scripts; //holds all instances of player made scripts
	bool script_profiling;
	//A new script will be crated and added to scripts vector
	virtual void NewScript(); 
private:
	Clock::time_point startScripts; //time just before first script executed
	Clock::time_point endScripts; //time just after last script executed itself
	Clock::duration ScriptsRunTime; //total runtime of the scripts
};

