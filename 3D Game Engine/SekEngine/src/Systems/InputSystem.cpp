#include "InputSystem.h"
std::map <int, char_and_sf> InputSystem::keys;
std::map <int, mouse_and_sf> InputSystem::mouse;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::Init_Subsystem()
{
	SubSystem_name = "Input";
	load_settings();
}

void InputSystem::load_settings()
{
	//local transaltor between ASCII and SFML keys sicne the int value and enum value do not match up
	const std::map <char, sf::Keyboard::Key> ASCII_TO_SFML_KEY = {
		{'Q',sf::Keyboard::Q},
		{'W',sf::Keyboard::W},
		{'E',sf::Keyboard::E},
		{'R',sf::Keyboard::R},
		{'T',sf::Keyboard::T},
		{'Y',sf::Keyboard::Y},
		{'U',sf::Keyboard::U},
		{'I',sf::Keyboard::I},
		{'O',sf::Keyboard::O},
		{'P',sf::Keyboard::P},
		{'A',sf::Keyboard::A},
		{'S',sf::Keyboard::S},
		{'D',sf::Keyboard::D},
		{'F',sf::Keyboard::F},
		{'G',sf::Keyboard::G},
		{'H',sf::Keyboard::H},
		{'J',sf::Keyboard::J},
		{'K',sf::Keyboard::K},
		{'L',sf::Keyboard::L},
		{'Z',sf::Keyboard::Z},
		{'X',sf::Keyboard::X},
		{'C',sf::Keyboard::C},
		{'V',sf::Keyboard::V},
		{'B',sf::Keyboard::B},
		{'N',sf::Keyboard::N},
		{'M',sf::Keyboard::M},
		{' ',sf::Keyboard::Space},

		{',',sf::Keyboard::Comma},
		{'.',sf::Keyboard::Period},
		{'~',sf::Keyboard::Tilde},

		{'-',sf::Keyboard::F2},
		{'-',sf::Keyboard::F3},
		{'-',sf::Keyboard::Enter},
		{'-',sf::Keyboard::Up},
		{'-',sf::Keyboard::Right},
		{'-',sf::Keyboard::Left},
		{'-',sf::Keyboard::Down},
		{'-',sf::Keyboard::PageUp},
		{'-',sf::Keyboard::PageDown},
		{'-',sf::Keyboard::LControl},
		{'-',sf::Keyboard::RControl},
		{'-',sf::Keyboard::Enter}
	};

	//these are all default inputs for keyboard and mouse will be overwritten when things are loaded
	keys = {
			{input_key::FORWARD,char_and_sf('W',sf::Keyboard::W,input_key::FORWARD)},
			{input_key::LEFT,char_and_sf('A',sf::Keyboard::A,input_key::LEFT)},
			{input_key::BACKWARD,char_and_sf('S',sf::Keyboard::S,input_key::BACKWARD)},
			{input_key::RIGHT,char_and_sf('D',sf::Keyboard::D,input_key::RIGHT)},
			{input_key::JUMP,char_and_sf(' ',sf::Keyboard::Space,input_key::JUMP)},
			{input_key::USE,char_and_sf('E',sf::Keyboard::E,input_key::USE)},
			{input_key::SPRINT,char_and_sf(0,sf::Keyboard::LShift,input_key::SPRINT)},
			{input_key::CROUCH,	char_and_sf(0,sf::Keyboard::LControl,input_key::CROUCH)},

			{input_key::FORW_SECONDARY,char_and_sf(0,sf::Keyboard::Up,input_key::FORW_SECONDARY)},
			{input_key::LEFT_SECONDARY,char_and_sf(0,sf::Keyboard::Left,input_key::LEFT_SECONDARY)},
			{input_key::BACKW_SECONDARY,char_and_sf(0,sf::Keyboard::Down,input_key::BACKW_SECONDARY)},
			{input_key::RIGHT_SECONDARY,char_and_sf(0,sf::Keyboard::Right,input_key::RIGHT_SECONDARY)},
			{input_key::CHANGE,char_and_sf(0,sf::Keyboard::RControl,input_key::CHANGE)},
			{input_key::INCREASE,char_and_sf(0,sf::Keyboard::PageUp,input_key::INCREASE)},
			{input_key::DECREASE,char_and_sf(0,sf::Keyboard::PageDown,input_key::DECREASE)},
			{input_key::TAB,char_and_sf(0,sf::Keyboard::Tab,input_key::TAB)},
			{input_key::START,char_and_sf(0,sf::Keyboard::End,input_key::START)},
			{input_key::PREVIOUS,char_and_sf(0,sf::Keyboard::Comma,input_key::PREVIOUS)},
			{input_key::NEXT,char_and_sf(0,sf::Keyboard::Period,input_key::NEXT)},
			{input_key::ENTER,char_and_sf(0,sf::Keyboard::Enter,input_key::ENTER)},
			{input_key::CREATE_NEW,	char_and_sf('N',sf::Keyboard::N,input_key::CREATE_NEW)},
			{input_key::PROFILING,char_and_sf(0,sf::Keyboard::Tilde,input_key::PROFILING)},
			{input_key::FPS_TOGGLE,char_and_sf(0,sf::Keyboard::F2,input_key::FPS_TOGGLE)},
			{input_key::DEBUGGING,char_and_sf(0,sf::Keyboard::F3,input_key::DEBUGGING)},

			{input_key::ZOOM_IN,char_and_sf(0,sf::Keyboard::Unknown,input_key::ZOOM_IN)},
			{input_key::ZOOM_OUT,char_and_sf(0,sf::Keyboard::Unknown,input_key::ZOOM_OUT)}
	};
	mouse = {
			{input_buttons::FIRE,mouse_and_sf(sf::Mouse::Button::Left,input_buttons::FIRE)},
			{input_buttons::SCOPE,mouse_and_sf(sf::Mouse::Button::Right,input_buttons::SCOPE)}
	};

	//load settings and overwrite them
	lua_State* L = luaL_newstate();
	luaL_dofile(L, "settings.lua");
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);

	std::vector<std::string> key_elements = Lua_load_file(L, "settings.lua", "keys");
	LuaRef table_name = getGlobal(L, "keys");

	char character;

	for (int i = 0; i < key_elements.size(); i++)
	{
		LuaRef entityCheck = table_name[key_elements.at(i)];
		character = entityCheck["char"].cast<char>();
		if (character > 'Z') {//lowercase to uppercase conversion
			character -= 32;
		}
		keys[entityCheck["enum"].cast<int>()].character = character;
		keys[entityCheck["enum"].cast<int>()].sf_key = ASCII_TO_SFML_KEY.at(character);
	}
}

void InputSystem::Update_Subsystem()
{
	Event* event;
	for (int i = 0; i < input_key::STANDARD_INPUT_SIZE; ++i) {
		if (sf::Keyboard::isKeyPressed(keys[i].sf_key)) {
			//when creating the array make sure the subsystems are in order, smallest to highest, otherwise they could be delayed by up to SubSystems number frames
			int temp_event_sub[] = { Event::Graphics, Event::Physics };
			if (keys[i].key_enum < input_key::FORW_SECONDARY) {
				event = new EventKeyPress(Event::EventType::Move_player, temp_event_sub, keys[i]);
			}
			else {
				if (level_editing) {
					int temp_event_sub[] = { Event::Engine };
					event = new EventKeyPress(Event::EventType::Move_object, temp_event_sub, keys[i]);
				}
				else {
					continue;
				}
			}
			EventHandler::add_event(event);
			keys[i].is_pressed = true;
			continue;
		}
		keys[i].is_pressed = false;
	}

	for (int i = input_key::STANDARD_INPUT_SIZE; i < input_key::EXTENDED_INPUT_SIZE; ++i) {
		if (sf::Keyboard::isKeyPressed(keys[i].sf_key)) {
			//when creating the array make sure the subsystems are in order, smallest to highest
			if (!keys[i].is_pressed) {
				int temp_event_sub[] = { Event::Engine };
				event = new EventKeyPress(Event::EventType::Move_object, temp_event_sub, keys[i]);
				EventHandler::add_event(event);
			}
			keys[i].is_pressed = true;
			continue;
		}
		keys[i].is_pressed = false;
	}

	if (level_editing) {
		if (sf::Keyboard::isKeyPressed(keys[input_key::START].sf_key)) {
			if (!keys[input_key::START].is_pressed) {
				int temp_event_sub[] = { Event::Physics };
				event = new Event(Event::EventType::ChangeSimState, temp_event_sub);
				EventHandler::add_event(event);
				keys[input_key::START].is_pressed = true;
			}
		}
		else {
			keys[input_key::START].is_pressed = false;
		}
	}

	if (level_editing) {
		if (sf::Keyboard::isKeyPressed(keys[input_key::ENTER].sf_key)) {
			if (!keys[input_key::ENTER].is_pressed) {
				int temp_event_sub[] = { Event::Engine };
				event = new Event(Event::EventType::SaveObject, temp_event_sub);
				EventHandler::add_event(event);
				keys[input_key::ENTER].is_pressed = true;
			}
		}
		else {
			keys[input_key::ENTER].is_pressed = false;
		}
	}

	if (level_editing) {
		if (sf::Keyboard::isKeyPressed(keys[input_key::PROFILING].sf_key)) {
			if (!keys[input_key::PROFILING].is_pressed) {
				SubSystem::Use_Profiling = !SubSystem::Use_Profiling;
				keys[input_key::PROFILING].is_pressed = true;
			}
		}
		else {
			keys[input_key::PROFILING].is_pressed = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(keys[input_key::CREATE_NEW].sf_key)) {
		if (!keys[input_key::CREATE_NEW].is_pressed) {
			keys[input_key::CREATE_NEW].is_pressed = true;
			int temp_event_sub[] = { Event::Engine };
			Event* event = new Event(Event::EventType::Create_new, temp_event_sub);
			EventHandler::add_event(event);
		}
	}
	else {
		keys[input_key::CREATE_NEW].is_pressed = false;
	}

	if (sf::Keyboard::isKeyPressed(keys[input_key::DEBUGGING].sf_key)) {
		if (!keys[input_key::DEBUGGING].is_pressed) {
			SubSystem::Debugging = !SubSystem::Debugging;
			keys[input_key::DEBUGGING].is_pressed = true;
		}
	}
	else {
		keys[input_key::DEBUGGING].is_pressed = false;
	}

	if (sf::Keyboard::isKeyPressed(keys[input_key::FPS_TOGGLE].sf_key)) {
		if (!keys[input_key::FPS_TOGGLE].is_pressed) {
			SubSystem::show_fps = !SubSystem::show_fps;
			keys[input_key::FPS_TOGGLE].is_pressed = true;
		}
	}
	else {
		keys[input_key::FPS_TOGGLE].is_pressed = false;
	}

	if (sf::Keyboard::isKeyPressed(keys[input_key::JUMP].sf_key)) {
		int temp_event_sub[] = { Event::Sound };
		Event* event = new Event(Event::EventType::Trigger, temp_event_sub);
		EventHandler::add_event(event);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		int temp_event_sub[] = { Event::Engine };
		Event* event = new Event(Event::EventType::Terminate, temp_event_sub);
		EventHandler::add_event(event);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { //hardcoded save shortcut
		int temp_event_sub[] = { Event::Engine };
		Event* event = new Event(Event::EventType::Save, temp_event_sub);
		EventHandler::add_event(event);
		std::cout << "Saving" << std::endl;
	}

	for (int i = 0; i < input_buttons::BUTTONS_SIZE; ++i) {
		if (sf::Mouse::isButtonPressed(mouse[i].sf_button)) {
			if (!mouse[i].is_clicked) {
				if (mouse[i].button_enum == input_buttons::FIRE) {
					int temp_event_sub[] = { Event::Physics, Event::Sound };
					event = new EventMouseClick(Event::EventType::Shoot, temp_event_sub, mouse[i]);
					EventHandler::add_event(event);
				}

				mouse[i].is_clicked = true;
			}
		}
		else {
			mouse[i].is_clicked = false;
		}
	}

	/*
	Code bellow is what I used have when I sued sfml poll events, the key response was laggy and delayed even when used a map
	*/

	//sf::Event sfevent;
	//if (window->pollEvent(sfevent))
	//{
	//	switch (sfevent.type)
	//	{
	//	case sf::Event::MouseWheelMoved:
	//		if (sfevent.mouseWheel.delta > 0) {
	//			int temp_event_sub[] = { Event::Graphics };
	//			Event* event = new EventKeyPress(Event::EventType::Key_press, temp_event_sub, keys[input_key::ZOOM_IN]);
	//			EventHandler::add_event(event);
	//		}
	//		else {
	//			int temp_event_sub[] = { Event::Graphics };
	//			Event* event = new EventKeyPress(Event::EventType::Key_press, temp_event_sub, keys[input_key::ZOOM_OUT]);
	//			EventHandler::add_event(event);
	//		}
	//		break;
	//	case sf::Event::MouseButtonPressed:
	//		std::cout << sf::Mouse::getPosition(*window).x << "\n" << sf::Mouse::getPosition(*window).y << "\n";
	//		break;
	//	case sf::Event::KeyPressed:
	//		for (int i = 0; i < STANDARD_INPUT_SIZE; ++i) {
	//			if (sfevent.key.code == keys[i].sf_key) {
	//				keys[i].is_pressed = true;
	//			}
	//		}
	//		if (sfevent.key.code == sf::Keyboard::Escape) {
	//			int temp_event_sub[] = { Event::Engine };
	//			Event* event = new Event(Event::EventType::Terminate, temp_event_sub);
	//			EventHandler::add_event(event);
	//		}
	//		if (sfevent.key.control && sfevent.key.code == sf::Keyboard::S) { //hardcoded save shortcut
	//			int temp_event_sub[] = { Event::Engine };
	//			Event* event = new Event(Event::EventType::Save, temp_event_sub);
	//			EventHandler::add_event(event);
	//			std::cout << "Saving" << std::endl;
	//		}
	//		if (sfevent.key.code == sf::Keyboard::Space) { //hardcoded save shortcut
	//			int temp_event_sub[] = { Event::Sound };
	//			Event* event = new Event(Event::EventType::Trigger, temp_event_sub);
	//			EventHandler::add_event(event);
	//		}
	//		break;
	//	case sf::Event::KeyReleased:
	//		for (int i = 0; i < STANDARD_INPUT_SIZE; ++i) {
	//			if (sfevent.key.code == keys[i].sf_key) {
	//				keys[i].is_pressed = false;
	//			}
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//}

	Process_events();
	Profiling();
}

void InputSystem::Process_events()
{
	for (int i = 0; i < EventHandler::EventQueue.size(); ++i) {
		for (int j = 0; Event::Subsystems_size; ++j) {
			if (EventHandler::EventQueue[i]->subsystems_involved[j] == Event::Input) {
				switch (EventHandler::EventQueue[i]->type)
				{
				case Event::EventType::Key_press:
					output_key_pressed(EventHandler::EventQueue[i]);
					break;
				default:
					std::cout << "Input function enum out of range" << std::endl;
					break;
				}
				EventHandler::EventQueue[i]->subsystems_involved[j] = Event::NoSubSystem;
			}
			else if (EventHandler::EventQueue[i]->subsystems_involved[j] > Event::Input ||
				EventHandler::EventQueue[i]->subsystems_involved[j] == Event::End) {
				break;
			}
		}
	}
}

void InputSystem::output_key_pressed(Event* event)
{
	if (SubSystem::Debugging) {
		char_and_sf* missingno = (char_and_sf*)event->get_value();
		std::cout << "Event of type: " << event->type << " is executed with value: " << missingno->character << std::endl;
	}
}