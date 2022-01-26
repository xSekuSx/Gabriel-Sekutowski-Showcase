#include "Event.h"

Event::Event(int event_type, int arr[Event::SubSystems::Subsystems_size])
{
    std::fill_n(subsystems_involved, Subsystems_size, Event::End); //make the list filled with -1s
	type = event_type; //copy the event type
    for (int i = 0; i < Event::SubSystems::Subsystems_size; ++i) { //then copy all the values
        if (arr[i] < -1) {
            break;
        }
        subsystems_involved[i] = arr[i];
    }
}

Event::~Event()
{
}

void Event::add_subsystem(int subsystem)
{
	//loops trough the array and finds the right place to put the subsystem, so it is in ascending order
	for (int i = 0; i < Event::Subsystems_size; ++i) {
		if (this->subsystems_involved[i] == Event::End) {
			this->subsystems_involved[i] = subsystem;
			return;
		}
		else if (this->subsystems_involved[i] == Event::NoSubSystem) {
			if (this->subsystems_involved[i + 1] > subsystem) {
				this->subsystems_involved[i] = subsystem;
				return;
			}
		}
	}
	std::cout << "Error didn't add new subsystem" << std::endl;
}

const void* Event::get_value()
{
    //std::cout << "trying to get a value from a default event class" << std::endl;
    return value;
}

void Event::set_value(const void* new_value)
{
    value = new_value;
	delete_value = true;
}
