#include "EventHandler.h"

std::vector<Event*> EventHandler::EventQueue;

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::add_event(Event* event)
{
	EventQueue.push_back(event);
}

void EventHandler::clear_events()
{
	bool empty;
	for (int i = 0; i < EventHandler::EventQueue.size(); ++i) {
		empty = true;
		for (int j = 0; Event::Subsystems_size; ++j) {
			if (EventHandler::EventQueue[i]->subsystems_involved[j] > Event::End) { //if it hits an end, break, end of list
				if (EventHandler::EventQueue[i]->subsystems_involved[j] > Event::NoEvent) { //if anysubsystem is present it is not empty
					empty = false;
				}
			}
			else {
				break;
			}
		}
		if (empty) {
			if (EventQueue[i]->delete_value) { //events can hold new pointer values if they do, delete the value
				delete EventQueue[i]->get_value();
			}
			delete EventQueue[i];
			EventQueue.erase(EventQueue.begin() + i);
		}
	}
}

void EventHandler::clear_all_events()
{
	for (int i = 0; i < EventQueue.size(); ++i) {
		if (EventQueue[i]->delete_value) {
			delete EventQueue[i]->get_value();
		}
		delete EventQueue[i];
	}
	EventQueue.clear();
}
