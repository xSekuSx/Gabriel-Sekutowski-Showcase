#include "EventPress.h"

EventKeyPress::EventKeyPress(int event_type, int arr[Subsystems_size], char_and_sf key) : Event(event_type, arr)
{
	key_pressed = key;
}

const void* EventKeyPress::get_value()
{
	return &key_pressed;
}

EventKeyPress::~EventKeyPress()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EventMouseClick::EventMouseClick(int event_type, int arr[Subsystems_size], mouse_and_sf button) : Event(event_type, arr)
{
	button_clicked = button;
}

EventMouseClick::~EventMouseClick()
{
}

const void* EventMouseClick::get_value()
{
	return &button_clicked;
}
