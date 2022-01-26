#pragma once
#include "Event.h"
#include <vector>
class EventHandler
{
public:
	EventHandler();
	~EventHandler();
	//add an event to the list, make sure it has right subsystems adn they are in ascending order
	static void add_event(Event* event);
	//call this function once per frame, clears empty events
	static void clear_events();
	//call this functipon at the deconstruction, clears all the events regardless if they have been executed or not
	static void clear_all_events();
	static std::vector<Event*> EventQueue;
private:
};

