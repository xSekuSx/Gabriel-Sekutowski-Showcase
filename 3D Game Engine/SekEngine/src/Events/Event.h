#pragma once
#include <iostream>
#include <string>
#include <algorithm>

class Event
{
public:
	enum SubSystems { End = -1, NoSubSystem = 0, Engine, Graphics, Input, Physics, Sound, Subsystems_size }; //all different subsystems in the game that can receive events
	enum EventType { NoEvent = 0, Key_press, Move_player, Move_object, ChangeObject, SaveObject, Save, Load, Trigger, ChangeSimState, RayCast, Shoot, SendScore, DisplayTop5Names, DisplayTop5Scores, NewLevel, Pause, Resume, Create_new, Add_model_node, Add_SekRigidBody, Add_sound_agent, Terminate }; //all different event types
	int subsystems_involved[Subsystems_size]; //each event will have an array of subsystem size
	int type = EventType::NoEvent;
	bool delete_value = false; //this is crucial variable, if a "new" value is created and added to the event, this will tell the deconstructor to delete that value aswell

	Event(int event_type, int arr[Subsystems_size]);
	~Event();
	//this will work fine as long as there are no duplicates in the event subsystems i.e. Input twice //there shoudln't be any duplicates
	//loops trough the array and finds the right place to put the subsystem, so it is in ascending order
	virtual void add_subsystem(int subsystem);
	//gets the void pointer value use it only when you know what you send
	virtual const void* get_value();
	//this function will set delete_value to true, don't set it equal to false unless you delete the data somehwere else yourself
	virtual void set_value(const void* new_value);
private:
	const void* value = NULL;
};