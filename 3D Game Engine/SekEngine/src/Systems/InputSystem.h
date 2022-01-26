#pragma once
#include <map>
#include "SubSystem.h"

class InputSystem : SubSystem
{
public:
	static std::map <int, char_and_sf> keys; //Map that translates inputKey enum to corresponding struct which holds the right sfml key
	static std::map <int, mouse_and_sf> mouse; //Map that translates mouseButton enum to corresponding struct which holds the right sfml button
	InputSystem();
	~InputSystem();
	void Init_Subsystem();
	void Process_events();
	void Update_Subsystem();
	void output_key_pressed(Event* event);
	//load user key bindings settings
	static void load_settings();
private:
};