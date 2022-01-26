#pragma once
#include "Event.h"
#include <SFML/Graphics.hpp>

enum input_key : int { NoInput = -1, FORWARD = 0, LEFT, BACKWARD, RIGHT, JUMP, CROUCH, SPRINT, USE, FORW_SECONDARY, LEFT_SECONDARY, BACKW_SECONDARY, RIGHT_SECONDARY, STANDARD_INPUT_SIZE, CHANGE, INCREASE, DECREASE, ZOOM_IN, ZOOM_OUT, NEXT, PREVIOUS, EXTENDED_INPUT_SIZE, ENTER, TAB, START, CREATE_NEW, PROFILING, DEBUGGING, FPS_TOGGLE, TOTAL_SIZE };
enum input_buttons : int {NoButton = -1, FIRE = 0, SCOPE, BUTTONS_SIZE};

struct char_and_sf {
	sf::Keyboard::Key sf_key;
	int key_enum;
	char character;
	bool is_pressed;
	
	char_and_sf(char ascii_character = 0, sf::Keyboard::Key key = sf::Keyboard::Unknown, int action_number = -1) {
		character = ascii_character;
		sf_key = key;
		key_enum = action_number;
		is_pressed = false;
	}
};

struct mouse_and_sf{
	sf::Mouse::Button sf_button;
	int button_enum;
	bool is_clicked;

	mouse_and_sf(sf::Mouse::Button button = sf::Mouse::Button::ButtonCount, int action_number = -1) {
		sf_button = button;
		button_enum = action_number;
		is_clicked = false;
	}
};

class EventKeyPress : public Event
{
public:
	char_and_sf key_pressed;
	EventKeyPress(int event_type, int arr[Subsystems_size], char_and_sf key);
	~EventKeyPress();
	const void* get_value();
};

class EventMouseClick : public Event
{
public:
	mouse_and_sf button_clicked;
	EventMouseClick(int event_type, int arr[Subsystems_size], mouse_and_sf button);
	~EventMouseClick();
	const void* get_value();
};

