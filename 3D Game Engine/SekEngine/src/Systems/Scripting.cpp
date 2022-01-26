#include "Scripting.h"

std::vector<Scripting*> Scripting::Scripts;

Scripting::Scripting()
{
}

Scripting::~Scripting()
{
}

void Scripting::Update()
{
	if (script_profiling) {
		startScripts = Clock::now();
	}
	for (int i = 0; i < Scripts.size(); ++i) {
		Scripts[i]->Update();
	}
	if (script_profiling) {
		endScripts = Clock::now();
		ScriptsRunTime = endScripts - startScripts;
		std::cout << "=================================================================" << "\n";
		std::cout << "SubSystem: Scripts\n";
		std::cout << "Time taken to run Scripts in nano seconds: " << ScriptsRunTime.count() << "\n";
		std::cout << "Time taken to run Scripts in milliseconds: " << ScriptsRunTime.count() * 0.000001 << "\n";
		std::cout << "Time taken to run Scripts in seconds: " << ScriptsRunTime.count() * 0.000000001 << "\n";
	}
}

void Scripting::Destroy()
{
}

void Scripting::delete_scripts()
{
	for (int i = 0; i < Scripts.size(); ++i) {
		Scripts[i]->Destroy();
	}
	Scripts.clear();
}

void Scripting::show_profiling(bool state)
{
	script_profiling = state;
}

void Scripting::NewScript()
{
	Scripts.push_back(this);
}
