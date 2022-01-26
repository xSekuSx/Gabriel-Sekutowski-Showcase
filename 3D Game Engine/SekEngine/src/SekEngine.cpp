#include "SekEngine.h"

/*
Default keys in game:
WASD - movement
Mouse1 - firing

Default movement in the edittor:

End switches between level editing adn level testing - runs a simulation
Enter to apply changes done to the gameObject in level editing
Saving possible trough L Control + S

arrows - moves the object
right control - switches up and down with forward and backwards
,. - switches between left and right gameObject
F2 - debug toggle, will print out raycasts
F3 - FPS toggle
~ - profiling toggle
N - creates new gameObject
Space - plays a mono sound
Page Up & Down - increase / decrease editting step
*/

GraphicsSystem graphics_system;
PhysicsSystem physiscs_system;
InputSystem input_system;
SoundSystem sound_system;
Networking network;

Scripting scripts;

Player player;

void SekEngine::Initialise_subsystems()
{
	SubSystem::CurrentTime = Clock::now();
	SubSystem::Debugging = get_default_bool(options::Initialise_Debugging);
	SubSystem::level_editing = get_default_bool(options::Level_editing);

	int resX = get_default_int(options::ResolutionX);
	int resY = get_default_int(options::ResolutionY);
	bool full_screen = get_default_bool(options::Full_screen);
	graphics_system.Init_Subsystem(resX, resY, full_screen);
	input_system.Init_Subsystem();
	physiscs_system.Init_Subsystem();
	sound_system.Init_Subsystem();
	IncludeScripts::IncludeAll();

	levels_sequence = get_default_vector(options::Levels_list); //get the lists of levels to load from default.json

	int temp_event_sub[] = { Event::Engine };
	Event* event = new Event(Event::EventType::Load, temp_event_sub);
	event->set_value(static_cast<void*>(new std::string(levels_sequence[0])));
	EventHandler::add_event(event);

	levels_sequence.erase(levels_sequence.begin());
}

void SekEngine::Update_loop()
{
	while (running) {
		Calculate_delta_time();

		if (game_paused) { //if game pause just loop trough the graphics to see if it is active again and reset deltatime
			Clock::duration delta_copy = SubSystem::DeltaTime;
			while (game_paused) {
				graphics_system.Update_Subsystem();
				SekEngine::Process_events();
				EventHandler::clear_events();
			}
			SubSystem::DeltaTime = delta_copy;
			SubSystem::CurrentTime = Clock::now();
		}

		graphics_system.Update_Subsystem();
		physiscs_system.Update_Subsystem();
		input_system.Update_Subsystem();
		sound_system.Update_Subsystem();
		scripts.Update();
		SekEngine::Process_events();
		EventHandler::clear_events();
	}
}

void SekEngine::Deactivate_subsytems() {
	EventHandler::clear_all_events();
	SekEngine::Delete_game_objects();
	network.destroy();
	scripts.delete_scripts();

	//used for checking memory leaks
	//they will destroy themselves at the end of the program, calling them now will create errors
	//graphics_system.~GraphicsSystem();
	//physiscs_system.~PhysicsSystem();
	//sound_system.~SoundSystem();
	//input_system.~InputSystem();
}

void SekEngine::Calculate_delta_time() {
	SubSystem::OldTime = SubSystem::CurrentTime;
	SubSystem::CurrentTime = Clock::now();
	SubSystem::DeltaTime = SubSystem::CurrentTime - SubSystem::OldTime;
	SubSystem::DeltaTimeMS = SubSystem::DeltaTime.count() * 0.000001;

	SubSystem::Timer = Clock::now() - SubSystem::GameStartTime;

	scripts.show_profiling(SubSystem::Second_passed && SubSystem::Use_Profiling);
	if (SubSystem::show_fps || SubSystem::Use_Profiling) {
		accumulated_DeltaTime += SubSystem::DeltaTimeMS;
		if (accumulated_DeltaTime > 1000) { //update once every second
			accumulated_DeltaTime = 0;
			SubSystem::Second_passed = true;
			SubSystem::FPS = static_cast<int>(1 / (SubSystem::DeltaTime.count() * 0.000000001));
			SubSystem::CurrentSubTime = Clock::now();
		}
		else {
			SubSystem::Second_passed = false;
		}
	}

	if (SubSystem::show_fps && SubSystem::Second_passed) {
		std::cout << "\n";
		std::cout << "=================================================================\n";
		std::cout << "Time between frames in nano seconds: " << SubSystem::DeltaTime.count() << "\n";
		std::cout << "Time taken for this frame in milliseconds: " << SubSystem::DeltaTimeMS << "\n";
		std::cout << "FPS: " << SubSystem::FPS << "\n";
		std::cout << "=================================================================" << "\n";
		std::cout << "\n";
	}
}

void SekEngine::Process_events() {
	for (int i = 0; i < EventHandler::EventQueue.size(); ++i) {
		for (int j = 0; Event::Subsystems_size; ++j) {
			if (EventHandler::EventQueue[i]->subsystems_involved[j] == Event::Engine) {
				switch (EventHandler::EventQueue[i]->type)
				{
				case Event::EventType::Terminate:
					running = false;
					break;
				case Event::EventType::Save:
					Save_level();
					break;
				case Event::EventType::SaveObject:
					Save_current_object();
					break;
				case Event::EventType::Move_object:
					Move_object(EventHandler::EventQueue[i]);
					break;
				case Event::EventType::ChangeObject:
					Change_current_object_id(EventHandler::EventQueue[i]);
					break;
				case Event::EventType::Load:
					Load_scene_gameobjects(EventHandler::EventQueue[i]);
					break;
				case Event::EventType::SendScore:
					DisplayScores();
					break;
				case Event::EventType::NewLevel:
					Load_new_level();
					return;
					break;
				case Event::EventType::Pause:
					game_paused = true;
					break;
				case Event::EventType::Resume:
					game_paused = false;
					break;
				case Event::EventType::Create_new:
					Add_new_gameObject();
					break;
				default:
					std::cout << "Game Engine function enum out of range" << std::endl;
					break;
				}
				EventHandler::EventQueue[i]->subsystems_involved[j] = Event::NoSubSystem;
			}
			else if (EventHandler::EventQueue[i]->subsystems_involved[j] > Event::Engine ||
				EventHandler::EventQueue[i]->subsystems_involved[j] == Event::End) {
				break;
			}
		}
	}
}

void SekEngine::Load_new_level()
{
	if (levels_sequence.size() == 0) {
		DisplayScores();
		return;
	}

	EventHandler::clear_all_events();
	SekEngine::Delete_game_objects();
	graphics_system.delete_models();
	graphics_system.add_camera();
	physiscs_system.delete_world_objects();

	int temp_event_sub[] = { Event::Engine };
	Event* event = new Event(Event::EventType::Load, temp_event_sub);
	event->set_value(static_cast<void*>(new std::string(levels_sequence[0])));
	EventHandler::add_event(event);

	levels_sequence.erase(levels_sequence.begin());
}

void SekEngine::Load_scene_gameobjects(Event* event)
{
	std::cout << "Loading..." << std::endl;
	std::string* scene_name = (std::string*)(event->get_value());
	std::string load_path = get_file_path();
	load_path += "\\Levels\\" + *scene_name;
	load_scene_from_file(load_path);

	//instead of creating new event, add new subsystems to the load event
	event->add_subsystem(Event::SubSystems::Graphics);
	event->add_subsystem(Event::SubSystems::Physics);
	event->add_subsystem(Event::SubSystems::Sound);
}

void SekEngine::Save_level()
{
	std::string save_path = get_file_path();
	save_path += "\\Levels\\" + (get_default_string(options::Save));
	save_scene_to_file(save_path);
}

void SekEngine::DisplayScores()
{
	std::cout << "End Game" << std::endl;
	float finalTime = SubSystem::Timer.count() * 0.000000001;
	std::string nick = get_default_string(options::User_name);
	network.communicate(finalTime, nick); //send the data to the netweork that sends it to the server

	//get the final results and send it to graphics
	std::vector<float> top5_scores = network.top5_final_scores;
	int temp_event_sub[] = { Event::Graphics };
	Event* event = new Event(Event::EventType::DisplayTop5Scores, temp_event_sub);
	event->set_value(static_cast<std::vector<float>*>(new std::vector<float>(top5_scores)));
	EventHandler::add_event(event);

	std::vector<std::string> top5_names = network.top5_final_players;
	int temp_event_sub2[] = { Event::Graphics };
	Event* event2 = new Event(Event::EventType::DisplayTop5Names, temp_event_sub2);
	event2->set_value(static_cast<std::vector<std::string>*>(new std::vector<std::string>(top5_names)));
	EventHandler::add_event(event2);
}

void SekEngine::Change_current_object_id(Event* event)
{
	level_editing_model_selected = *(int*)event->get_value();
}

void SekEngine::Add_new_gameObject()
{
	GameObject* newGO = new GameObject();
}

void SekEngine::Delete_game_objects()
{
	for (int i = 0; i < GameObject::GameObjects.size(); ++i) {
		GameObject::GameObjects[i]->deleteComponents();
		delete GameObject::GameObjects[i];
	}
	GameObject::clear_vectors();
}

void SekEngine::Save_current_object()
{
	GameObject::GameObjects[level_editing_model_selected]->set_name(graphics_system.Graphical_interface.object_name);
	GameObject::GameObjects[level_editing_model_selected]->set_position(graphics_system.Graphical_interface.object_posVec);
	GameObject::GameObjects[level_editing_model_selected]->set_rotation(graphics_system.Graphical_interface.object_rotVec);
	GameObject::GameObjects[level_editing_model_selected]->set_scale(graphics_system.Graphical_interface.object_scaleVec);
	GameObject::GameObjects[level_editing_model_selected]->set_primitive(static_cast<GameObject::Primitives>(graphics_system.Graphical_interface.object_primitive));
	if (GameObject::GameObjects[level_editing_model_selected]->get_model_node()) {
		GameObject::GameObjects[level_editing_model_selected]->get_model_node()->set_model_mesh_path(graphics_system.Graphical_interface.object_model_mesh);
		GameObject::GameObjects[level_editing_model_selected]->get_model_node()->set_model_texture_path(graphics_system.Graphical_interface.object_model_texture);
		GameObject::GameObjects[level_editing_model_selected]->get_model_node()->set_position_offset(graphics_system.Graphical_interface.object_model_posVec);
		GameObject::GameObjects[level_editing_model_selected]->get_model_node()->set_rotational_offset(graphics_system.Graphical_interface.object_model_rotVec);
		GameObject::GameObjects[level_editing_model_selected]->get_model_node()->set_model_scale(graphics_system.Graphical_interface.object_model_scale);
		GameObject::GameObjects[level_editing_model_selected]->get_model_node()->set_GO_scale(GameObject::GameObjects[level_editing_model_selected]->get_scale());
	}
	if (GameObject::GameObjects[level_editing_model_selected]->get_rigidbody()) {
		GameObject::GameObjects[level_editing_model_selected]->get_rigidbody()->set_transform_offset(graphics_system.Graphical_interface.object_rigidBody_posVec);
		GameObject::GameObjects[level_editing_model_selected]->get_rigidbody()->set_rotation_offset(graphics_system.Graphical_interface.object_rigidBody_rotVec);
		GameObject::GameObjects[level_editing_model_selected]->get_rigidbody()->set_collider_scale(graphics_system.Graphical_interface.object_rigidBody_scale);
		GameObject::GameObjects[level_editing_model_selected]->get_rigidbody()->set_mass(graphics_system.Graphical_interface.object_mass);
		GameObject::GameObjects[level_editing_model_selected]->get_rigidbody()->set_GO_scale(GameObject::GameObjects[level_editing_model_selected]->get_scale());

		GameObject::GameObjects[level_editing_model_selected]->get_rigidbody()->set_transform_position(graphics_system.Graphical_interface.object_posVec); //need to apply offset to rigidbodies
	}
	if (GameObject::GameObjects[level_editing_model_selected]->get_sound_agent()) {
		GameObject::GameObjects[level_editing_model_selected]->get_sound_agent()->set_sound_name(graphics_system.Graphical_interface.object_sound_path);
		GameObject::GameObjects[level_editing_model_selected]->get_sound_agent()->mono_stereo = graphics_system.Graphical_interface.object_sound_mono_stereo;
	}
}

void SekEngine::Move_object(Event* event)
{
	char_and_sf* key_pressed = (char_and_sf*)event->get_value();

	switch (key_pressed->key_enum)
	{
	case input_key::NEXT:
		++level_editing_model_selected;
		if (level_editing_model_selected >= GameObject::GameObjects.size()) {
			level_editing_model_selected = 0;
		}
		graphics_system.Graphical_interface.set_current_obj_id(level_editing_model_selected);
		std::cout << "selected id: " << level_editing_model_selected << " " << GameObject::GameObjects[level_editing_model_selected]->get_name() << std::endl;
		next_object = true;
		graphics_system.Graphical_interface.next_object(true);
		return;

	case input_key::PREVIOUS:
		--level_editing_model_selected;
		if (level_editing_model_selected < 0) {
			level_editing_model_selected = GameObject::GameObjects.size() - 1;
		}
		graphics_system.Graphical_interface.set_current_obj_id(level_editing_model_selected);
		std::cout << "selected id: " << level_editing_model_selected << " " << GameObject::GameObjects[level_editing_model_selected]->get_name() << std::endl;
		next_object = true;
		graphics_system.Graphical_interface.next_object(true);
		return;

	case input_key::CHANGE: //change moving forward backwads to up and down with arrows;
		axis_forward_up = !axis_forward_up;
		if (axis_forward_up) {
			std::cout << "current axis are now Up" << std::endl;
		}
		else {
			std::cout << "current axis are now forward" << std::endl;
		}
		return;
	case input_key::INCREASE:
		move_object_multiplier *= 10;
		return;

	case input_key::DECREASE:
		move_object_multiplier /= 10;
		return;

	default:
		break;
	}

	Svec3 pos = GameObject::GameObjects[level_editing_model_selected]->get_position();
	Svec3 rot = GameObject::GameObjects[level_editing_model_selected]->get_rotation();

	switch (key_pressed->key_enum)
	{
	case input_key::FORW_SECONDARY:
		if (axis_forward_up) {
			pos = pos + Svec3(0, move_object_multiplier, 0);
		}
		else {
			pos = pos + Svec3(0, 0, move_object_multiplier);
		}
		break;
	case input_key::BACKW_SECONDARY:
		if (axis_forward_up) {
			pos = pos + Svec3(0, -move_object_multiplier, 0);
		}
		else {
			pos = pos + Svec3(0, 0, -move_object_multiplier);
		}
		break;
	case input_key::LEFT_SECONDARY:
		if (axis_forward_up) {
			rot = rot + Svec3(0, -1, 0);
		}
		else {
			pos = pos + Svec3(-move_object_multiplier, 0, 0);
		}
		break;
	case input_key::RIGHT_SECONDARY:
		if (axis_forward_up) {
			rot = rot + Svec3(0, 1, 0);
		}
		else {
			pos = pos + Svec3(move_object_multiplier, 0, 0);
		}
		break;
	default:
		break;
	}

	GameObject::GameObjects[level_editing_model_selected]->set_position(pos);
	GameObject::GameObjects[level_editing_model_selected]->set_rotation(rot);

	update_object = true;
	graphics_system.Graphical_interface.update_object(true);
}

int main() {
	SekEngine::Initialise_subsystems();
	SekEngine::Update_loop();
	SekEngine::Deactivate_subsytems();
	return 0;
}

/*
Default keys in game:
WASD - movement
Mouse1 - firing

Default movement in the edittor:

End switches between level editing adn level testing - runs a simulation
Enter to apply changes done to the gameObject in level editing
Saving possible trough L Control + S

arrows - moves the object
right control - switches up and down with forward and backwards
,. - switches between left and right gameObject
F2 - debug toggle, will print out raycasts
F3 - FPS toggle
~ - profiling toggle
N - creates new gameObject
Space - plays a mono sound
Page Up & Down - increase / decrease editting step
*/