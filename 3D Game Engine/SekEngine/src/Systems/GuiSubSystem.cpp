#include "GuiSubSystem.h"

std::string GuiSubSystem::object_name, GuiSubSystem::object_model_mesh, GuiSubSystem::object_model_texture, GuiSubSystem::object_sound_path;
float GuiSubSystem::display_pos[3], GuiSubSystem::display_model_pos[3], GuiSubSystem::display_rigidBody_pos[3];
float GuiSubSystem::display_rot[3], GuiSubSystem::display_model_rot[3], GuiSubSystem::display_rigidBody_rot[3];
float GuiSubSystem::display_scale[3], GuiSubSystem::display_model_scale[3], GuiSubSystem::display_rigidBody_scale[3];
Svec3 GuiSubSystem::object_posVec, GuiSubSystem::object_rotVec, GuiSubSystem::object_scaleVec, GuiSubSystem::object_model_posVec, GuiSubSystem::object_model_rotVec, GuiSubSystem::object_model_scale,
GuiSubSystem::object_rigidBody_posVec, GuiSubSystem::object_rigidBody_rotVec, GuiSubSystem::object_rigidBody_scale;
float GuiSubSystem::object_mass;
float* GuiSubSystem::display_mass;
int GuiSubSystem::object_primitive;
bool* GuiSubSystem::object_sound_mono_stereo;

GuiSubSystem::GuiSubSystem()
{
}

GuiSubSystem::~GuiSubSystem()
{
	imGUIdevice->drop();
}

void GuiSubSystem::Init_Subsystem()
{
	std::cout << "Initialising GUI" << std::endl;
	SubSystem_name = "GUI";

	imGUIdevice = IrrIMGUI::createIMGUI(device, &EventReceiver);
}

void GuiSubSystem::get_current_gameObject_values() {
	if (GameObject::GameObjects.size()) {
		object_posVec = GameObject::GameObjects[current_object_id]->get_position();
		object_rotVec = GameObject::GameObjects[current_object_id]->get_rotation();
		object_scaleVec = GameObject::GameObjects[current_object_id]->get_scale();
		display_pos[0] = object_posVec.x;
		display_pos[1] = object_posVec.y;
		display_pos[2] = object_posVec.z;
		display_rot[0] = object_rotVec.x;
		display_rot[1] = object_rotVec.y;
		display_rot[2] = object_rotVec.z;
		display_scale[0] = object_scaleVec.x;
		display_scale[1] = object_scaleVec.y;
		display_scale[2] = object_scaleVec.z;
		object_primitive = static_cast<int>(GameObject::GameObjects[current_object_id]->get_primitive());
	}
	if (GameObject::GameObjects.size() && (gui_next_object || gui_update_object)) {
		gui_next_object = false;
		gui_update_object = false;
		object_name = GameObject::GameObjects[current_object_id]->get_name();
		if (GameObject::GameObjects[current_object_id]->get_model_node()) {
			object_model_mesh = GameObject::GameObjects[current_object_id]->get_model_node()->get_model_mesh_path();
			object_model_texture = GameObject::GameObjects[current_object_id]->get_model_node()->get_texture_path();
			object_model_posVec = GameObject::GameObjects[current_object_id]->get_model_node()->get_position_offset();
			object_model_rotVec = GameObject::GameObjects[current_object_id]->get_model_node()->get_rotational_offset();
			object_model_scale = GameObject::GameObjects[current_object_id]->get_model_node()->get_model_scale();
			display_model_pos[0] = object_model_posVec.x;
			display_model_pos[1] = object_model_posVec.y;
			display_model_pos[2] = object_model_posVec.z;
			display_model_rot[0] = object_model_rotVec.x;
			display_model_rot[1] = object_model_rotVec.y;
			display_model_rot[2] = object_model_rotVec.z;
			display_model_scale[0] = object_model_scale.x;
			display_model_scale[1] = object_model_scale.y;
			display_model_scale[2] = object_model_scale.z;
		}
		if (GameObject::GameObjects[current_object_id]->get_rigidbody()) {
			object_rigidBody_posVec = GameObject::GameObjects[current_object_id]->get_rigidbody()->get_transform_offset();
			object_rigidBody_rotVec = GameObject::GameObjects[current_object_id]->get_rigidbody()->get_rotation_offset();
			object_rigidBody_scale = GameObject::GameObjects[current_object_id]->get_rigidbody()->get_collider_scale();
			display_rigidBody_pos[0] = object_rigidBody_posVec.x;
			display_rigidBody_pos[1] = object_rigidBody_posVec.y;
			display_rigidBody_pos[2] = object_rigidBody_posVec.z;
			display_rigidBody_rot[0] = object_rigidBody_rotVec.x;
			display_rigidBody_rot[1] = object_rigidBody_rotVec.y;
			display_rigidBody_rot[2] = object_rigidBody_rotVec.z;
			display_rigidBody_scale[0] = object_rigidBody_scale.x;
			display_rigidBody_scale[1] = object_rigidBody_scale.y;
			display_rigidBody_scale[2] = object_rigidBody_scale.z;
			object_mass = GameObject::GameObjects[current_object_id]->get_rigidbody()->get_mass();
			display_mass = &object_mass;
		}
		if (GameObject::GameObjects[current_object_id]->get_sound_agent()) {
			object_sound_path = GameObject::GameObjects[current_object_id]->get_sound_agent()->get_sound_name();
			object_sound_mono_stereo = &GameObject::GameObjects[current_object_id]->get_sound_agent()->mono_stereo;
		}
	}
}

void GuiSubSystem::Update_Subsystem()
{
	GuiOldTime = Clock::now();
	imGUIdevice->startGUI();

	if (SubSystem::show_fps) {
		ImGui::Begin("FPS");
		ImGui::Text(("FPS: " + FPS_string).c_str());
		ImGui::End();
	}

	if (show_final_game_scores) {
		ImGui::Begin("Top 5 Scores");
		ImGui::Text(table.c_str());
		ImGui::End();
	}

	if (!running_sim) {
		ImGui::Begin("Current GameObject");

		get_current_gameObject_values();

		ImGui::InputText("Name", const_cast<char*>(object_name.c_str()), sizeof(object_name));
		ImGui::DragFloat3("Position", display_pos, 0.5f);
		ImGui::DragFloat3("Rotation", display_rot, 0.5f);
		ImGui::DragFloat3("Scale", display_scale, 0.1f);
		ImGui::SliderInt("Primitive", &object_primitive, static_cast<int>(GameObject::Primitives::NoPrimitive), static_cast<int>(GameObject::Primitives::PrimitivesSize) - 1);
		if (GameObject::GameObjects.size() > 0) {
			int tag = (int)GameObject::GameObjects[current_object_id]->get_tag();
			std::string tag_string = "tag: " + std::to_string(tag);
			ImGui::Text(tag_string.c_str());
		}

		object_posVec = Svec3(display_pos[0], display_pos[1], display_pos[2]);
		object_rotVec = Svec3(display_rot[0], display_rot[1], display_rot[2]);
		object_scaleVec = Svec3(display_scale[0], display_scale[1], display_scale[2]);

		if (GameObject::GameObjects.size() > 0) {
			GameObject::GameObjects[current_object_id]->set_position(object_posVec);
			GameObject::GameObjects[current_object_id]->set_rotation(object_rotVec);
			GameObject::GameObjects[current_object_id]->set_scale(object_scaleVec);
			GameObject::GameObjects[current_object_id]->set_primitive(static_cast<GameObject::Primitives>(object_primitive));
		}

		if (ImGui::Button("Save Current GameObject", ImVec2(180, 20)))
		{
			int temp_event_sub[] = { Event::Engine };
			Event* event = new Event(Event::EventType::SaveObject, temp_event_sub);
			EventHandler::add_event(event);
		}

		if (GameObject::GameObjects.size() > 0) {
			if (GameObject::GameObjects[current_object_id]->get_model_node()) {
				if (ImGui::CollapsingHeader("ModelNode")) {
					ImGui::InputText("Model Path", const_cast<char*>(object_model_mesh.c_str()), sizeof(object_model_mesh));
					ImGui::InputText("Texture Path", const_cast<char*>(object_model_texture.c_str()), sizeof(object_model_texture));
					ImGui::DragFloat3("Model pos Offset", display_model_pos, 0.5f);
					ImGui::DragFloat3("Model rot Offset", display_model_rot, 0.5f);
					ImGui::DragFloat3("Model Scale", display_model_scale, 0.1f);

					object_model_posVec = Svec3(display_model_pos[0], display_model_pos[1], display_model_pos[2]);
					object_model_rotVec = Svec3(display_model_rot[0], display_model_rot[1], display_model_rot[2]);
					object_model_scale = Svec3(display_model_scale[0], display_model_scale[1], display_model_scale[2]);

					GameObject::GameObjects[current_object_id]->get_model_node()->set_position_offset(object_model_posVec);
					GameObject::GameObjects[current_object_id]->get_model_node()->set_rotational_offset(object_model_rotVec);
					GameObject::GameObjects[current_object_id]->get_model_node()->set_model_scale(object_model_scale);
				}
			}
			else {
				if (ImGui::Button("Add ModelNode", ImVec2(180, 20)))
				{
					int temp_event_sub[] = { Event::Graphics };
					Event* event = new Event(Event::EventType::Add_model_node, temp_event_sub);
					event->set_value(new int(current_object_id));
					EventHandler::add_event(event);
				}
			}
			if (GameObject::GameObjects[current_object_id]->get_rigidbody()) {
				if (ImGui::CollapsingHeader("SekRigidBody")) {
					ImGui::DragFloat3("RigidBody pos Offset", display_rigidBody_pos, 0.5f);
					ImGui::DragFloat3("RigidBody rot Offset", display_rigidBody_rot, 0.5f);
					ImGui::DragFloat3("RigidBody scale", display_rigidBody_scale, 0.1f);
					ImGui::InputFloat("Mass", display_mass);

					object_rigidBody_posVec = Svec3(display_rigidBody_pos[0], display_rigidBody_pos[1], display_rigidBody_pos[2]);
					object_rigidBody_rotVec = Svec3(display_rigidBody_rot[0], display_rigidBody_rot[1], display_rigidBody_rot[2]);
					object_rigidBody_scale = Svec3(display_rigidBody_scale[0], display_rigidBody_scale[1], display_rigidBody_scale[2]);

					GameObject::GameObjects[current_object_id]->get_rigidbody()->set_transform_position(object_rigidBody_posVec);
					GameObject::GameObjects[current_object_id]->get_rigidbody()->set_rotation_offset(object_rigidBody_rotVec);
					GameObject::GameObjects[current_object_id]->get_rigidbody()->set_collider_scale(object_rigidBody_scale);

					object_mass = *display_mass;
				}
			}
			else {
				if (ImGui::Button("Add SekRigidBody", ImVec2(180, 20)))
				{
					int temp_event_sub[] = { Event::Physics };
					Event* event = new Event(Event::EventType::Add_SekRigidBody, temp_event_sub);
					event->set_value(new int(current_object_id));
					EventHandler::add_event(event);
				}
			}
			if (GameObject::GameObjects[current_object_id]->get_sound_agent()) {
				if (ImGui::CollapsingHeader("SoundAgent")) {
					ImGui::InputText("Sound Path", const_cast<char*>(object_sound_path.c_str()),sizeof(object_sound_path));
					ImGui::Checkbox("Mono On Stereo Off",object_sound_mono_stereo);
				}
			}
			else {
				if (ImGui::Button("Add SoundAgent", ImVec2(180, 20)))
				{
					int temp_event_sub[] = { Event::Sound };
					Event* event = new Event(Event::EventType::Add_sound_agent, temp_event_sub);
					event->set_value(new int(current_object_id));
					EventHandler::add_event(event);
				}
			}
		}


		ImGui::End();

		ImGui::Begin("Hierarchy");

		ImGui::BeginChild("Hierarchy List");
		for (int n = 0; n < GameObject::GameObjects.size(); ++n) {
			bool pressed = false;
			ImGui::Selectable((std::to_string(GameObject::GameObjects[n]->get_id()) + " " + GameObject::GameObjects[n]->get_name()).c_str(), &pressed);
			if (pressed) {
				current_object_id = GameObject::GameObjects[n]->get_id();
				gui_next_object = true;
				int temp_event_sub[] = { Event::Engine };
				Event* event = new Event(Event::EventType::ChangeObject, temp_event_sub);
				event->set_value(static_cast<void*>(new int(current_object_id)));
				EventHandler::add_event(event);
			}
		}

		if (SubSystem::level_editing) {
			ImGui::Begin("Timer");
			ImGui::Text(("Time: " + Timer_string).c_str());
			ImGui::End();
		}

		ImGui::EndChild();
		ImGui::End();

	}

	if (SubSystem::level_editing) {
		imGUIdevice->drawAll();
	}
	Profiling();
}

void GuiSubSystem::Process_events()
{
}

void GuiSubSystem::Profiling()
{
	if (Use_Profiling && Second_passed) {
		GuiCurrentTime = Clock::now();
		GuiDeltaTime = GuiCurrentTime - GuiOldTime;
		if (Use_Profiling && Second_passed) {
			std::cout << "=================================================================" << "\n";
			std::cout << "SubSystem: " << SubSystem_name << "\n";
			std::cout << "Time taken between subsystems in nano seconds: " << GuiDeltaTime.count() << "\n";
			std::cout << "Time taken between subsystems in milliseconds: " << GuiDeltaTime.count() * 0.000001 << "\n";
			std::cout << "Time taken between subsystems in seconds: " << GuiDeltaTime.count() * 0.000000001 << "\n";
		}
	}
}

IrrIMGUI::CIMGUIEventReceiver& GuiSubSystem::get_EventReceiver()
{
	return EventReceiver;
}

void GuiSubSystem::send_irrDevice(irr::IrrlichtDevice* dev)
{
	device = dev;
}

void GuiSubSystem::next_object(bool val)
{
	gui_next_object = val;
}

void GuiSubSystem::update_object(bool val)
{
	gui_update_object = val;
}

void GuiSubSystem::set_fianal_scores(std::vector<float> top5scores)
{
	for (int i = 0; i < top5scores.size(); ++i) {
		scores.push_back(top5scores[i]);
	}
	if (scores.size() > 0 && names.size() > 0) {
		show_final_game_scores = true;
		for (int i = 0; i < 5; ++i) {
			table += names[i] + ": " + std::to_string(scores[i]) + "\n";
		}
	}
}

void GuiSubSystem::set_fianal_names(std::vector<std::string> top5names)
{
	for (int i = 0; i < top5names.size(); ++i) {
		names.push_back(top5names[i]);
	}
	if (scores.size() > 0 && names.size() > 0) {
		show_final_game_scores = true;
		for (int i = 0; i < 5; ++i) {
			table += names[i] + ": " + std::to_string(scores[i]) + "\n";
		}
	}
}

void GuiSubSystem::set_current_obj_id(int obj_id)
{
	current_object_id = obj_id;
}

void GuiSubSystem::setFPSstring(std::string newFPS)
{
	FPS_string = newFPS;
}

void GuiSubSystem::setTimer_string(std::string newTime)
{
	Timer_string = newTime;
}
