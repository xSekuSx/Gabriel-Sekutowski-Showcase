#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem()
{
}

GraphicsSystem::~GraphicsSystem()
{
	device->drop();
}

void GraphicsSystem::Init_Subsystem(int width, int height, bool full_screen)
{
	std::cout << "Initialising Graphics" << std::endl;
	SubSystem_name = "Graphics";
	WIDTH = width;
	HEIGHT = height;

	std::string default_path = get_file_path();
	missing_model_path = default_path + "\\Assets\\Default\\error.obj";
	missing_texture_path = default_path + "\\Assets\\Default\\MissingTexture Texture.png"; //Tom Daskevicious Idea, love you tom.

	Create_window(full_screen);
}

void GraphicsSystem::Create_window(bool full_screen)
{
	irr::SIrrlichtCreationParameters IrrlichtParams;
	IrrlichtParams.DriverType = irr::video::EDT_OPENGL;
	IrrlichtParams.WindowSize = irr::core::dimension2d<irr::u32>(WIDTH, HEIGHT);
	IrrlichtParams.Bits = 32;
	IrrlichtParams.Fullscreen = full_screen;
	IrrlichtParams.Stencilbuffer = true;
	IrrlichtParams.AntiAlias = 16;
	IrrlichtParams.Vsync = false;
	IrrlichtParams.EventReceiver = &Graphical_interface.get_EventReceiver();

	device = irr::createDeviceEx(IrrlichtParams);
	if (!device) {
		std::cout << "Problem with irrlicht window" << std::endl;
		return;
	}

	device->setWindowCaption(L"Sek Engine Demo");
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	geomtry_creator = smgr->getGeometryCreator();
	// Create GUI object

	Graphical_interface.send_irrDevice(device);
	Graphical_interface.Init_Subsystem();

	add_camera();

	if (Debugging) {
		FPS_display = L"FPS: ";
		FPS_string = std::to_string(FPS);
		Graphical_interface.setFPSstring(FPS_string);
		FPS_display += irr::core::stringw(FPS_string.c_str());
	}
	FPSTextBox = guienv->addStaticText(FPS_display.c_str(), irr::core::rect<irr::s32>(WIDTH - 70, 10, WIDTH - 10, 20), false);

	Timer_display = L"Timer: ";
	Timer_string = std::to_string(Timer.count() * 0.000000001); //conversion between nano seconds and seconds
	Graphical_interface.setTimer_string(Timer_string);
	Timer_display += irr::core::stringw(Timer_string.c_str());
	TimerTextBox = guienv->addStaticText(Timer_display.c_str(), irr::core::rect<irr::s32>(WIDTH * 0.5 - 40, 0, WIDTH * 0.5 + 40, 10), false); //put it in the middle +- 40 pixels

	//void* HWND = device->getVideoDriver()->getExposedVideoData().OpenGLWin32.HWnd;
	//handle = (sf::WindowHandle)HWND;
	//window = new sf::Window(handle);
}

void GraphicsSystem::add_camera()
{
	irr_maya_camera = smgr->addCameraSceneNodeMaya(0, -1000.f, 300.f, 500.f, -1, 70.f);
	irr_fps_camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.0f);

	if (!SubSystem::level_editing) { //when level editting maya camera
		irr_active_camera = irr_maya_camera;
		smgr->setActiveCamera(irr_active_camera);
	}
	else { //otherwise fps
		irr_active_camera = irr_fps_camera;
		device->getCursorControl()->setVisible(false);
		smgr->setActiveCamera(irr_active_camera);
	}

	Player::Players[0]->set_position(Svec3(0, -15, 0));
	irr_active_camera->setPosition(Svec3::Convert_to_irr(Player::Players[0]->get_position()));
}

void GraphicsSystem::add_model_node(Event* event)
{
	int id = *(int*)event->get_value();
	GameObject::GameObjects[id]->set_model_node(new ModelNode());
}

void GraphicsSystem::Update_Subsystem()
{
	//std::cout << "Updating Graphics" << std::endl;
	if (Player::Players.size() > 0) { //update the player forward vector and cameras position
		Player::Players[0]->set_forward(Svec3::Convert_from_irr(irr_active_camera->getTarget() - irr_active_camera->getPosition()).normalize());
		if (running_sim) {
			Player::Players[0]->update_camera_position();
			irr_active_camera->setPosition(Svec3::Convert_to_irr(Player::Players[0]->get_camera_position()));
		}
	}

	for (int i = 0; i < GameObject::GameObjects.size(); ++i) { //loop trough objects and if they have a model, update it
		if (GameObject::GameObjects[i]->get_model_node()) {
			GameObject::GameObjects[i]->get_model_node()->update_model(GameObject::GameObjects[i]->get_position(), GameObject::GameObjects[i]->get_rotation());
		}
	}

	if (device->run()) { //device run will do some irrlicht things and return if window is open or working/running otherwise false
		if (device->isWindowActive()) { //if window is active run things otherwise yeild
			if (!window_was_active) { //if it was active send a resume, so only 1 is sent
				window_was_active = true;
				int temp_event_sub[] = { Event::Engine };
				Event* event = new Event(Event::EventType::Resume, temp_event_sub);
				EventHandler::add_event(event);
			}
			driver->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));

			smgr->drawAll();

			if (SubSystem::show_fps) { //update fps
				if (SubSystem::Second_passed) {
					FPS_string = std::to_string(FPS);
					Graphical_interface.setFPSstring(FPS_string);
				}
				FPS_display = L"FPS: ";
				FPS_display += irr::core::stringw(FPS_string.c_str());
				FPSTextBox->setText(FPS_display.c_str());
			}
			else {
				FPS_display = L"";
				FPSTextBox->setText(FPS_display.c_str());
			}

			Timer_string = std::to_string(Timer.count() * 0.000000001);
			Graphical_interface.setTimer_string(Timer_string);
			Timer_display = L"Time: ";
			Timer_display += irr::core::stringw(Timer_string.c_str());
			TimerTextBox->setText(Timer_display.c_str());

			guienv->drawAll();

			Graphical_interface.Update_Subsystem();

			driver->endScene();

			if (running_sim) { //when in game switch to fps //would be best to move to event queue but no time
				irr_active_camera = irr_fps_camera;
				make_cursor_visible(false);
				smgr->setActiveCamera(irr_active_camera);
			}
			else { //when editting to maya
				irr_active_camera = irr_maya_camera;
				make_cursor_visible(true);
				smgr->setActiveCamera(irr_active_camera);
			}
		}
		else {
			device->yield();
			if (window_was_active) { //if it was active last time, send a pause event
				window_was_active = false;
				int temp_event_sub[] = { Event::Engine };
				Event* event = new Event(Event::EventType::Pause, temp_event_sub);
				EventHandler::add_event(event);
			}
		}
	}
	else {
		int temp_event_sub[] = { Event::Engine };
		Event* event = new Event(Event::EventType::Terminate, temp_event_sub);
		EventHandler::add_event(event);
	}

	Process_events();
	Profiling();
}

void GraphicsSystem::delete_models()
{
	smgr->clear();
}

void GraphicsSystem::make_cursor_visible(bool on_off)
{
	device->getCursorControl()->setVisible(on_off);
}

void GraphicsSystem::move_camera(Event* event)
{
	if (!running_sim) {
		char_and_sf* key_pressed = (char_and_sf*)event->get_value();

		Svec3 pos = Player::Players[0]->get_position();
		Svec3 dir = Player::Players[0]->get_forward_vector();
		Svec3 right_vector = Player::Players[0]->get_up_vector().cross(dir);
		Svec3 distance;

		if (key_pressed->key_enum == input_key::FORWARD) {
			distance = dir * movement_multiplier;
		}
		if (key_pressed->key_enum == input_key::ZOOM_IN) {
			distance = dir;
		}
		if (key_pressed->key_enum == input_key::BACKWARD) {
			distance = dir * -1 * movement_multiplier;
		}
		if (key_pressed->key_enum == input_key::ZOOM_OUT) {
			distance = dir * -1;
		}
		if (key_pressed->key_enum == input_key::LEFT) {
			distance = right_vector * -1 * movement_multiplier;
		}
		if (key_pressed->key_enum == input_key::RIGHT) {
			distance = right_vector * 1 * movement_multiplier;
		}

		pos = pos + distance;
		irr_active_camera->setPosition(Svec3::Convert_to_irr(pos));
		Player::Players[0]->set_position(pos);
	}
}

void GraphicsSystem::Process_events()
{
	for (int i = 0; i < EventHandler::EventQueue.size(); ++i) {
		for (int j = 0; Event::Subsystems_size; ++j) {
			if (EventHandler::EventQueue[i]->subsystems_involved[j] == Event::Graphics) {
				switch (EventHandler::EventQueue[i]->type)
				{
				case Event::EventType::Move_player:
					move_camera(EventHandler::EventQueue[i]);
					break;
				case Event::EventType::Load:
					Load_scene_models();
					break;
				case Event::EventType::Add_model_node:
					add_model_node(EventHandler::EventQueue[i]);
					break;
				case Event::EventType::DisplayTop5Names:
					Graphical_interface.set_fianal_names(*(std::vector<std::string>*)(EventHandler::EventQueue[i]->get_value()));
					break;
				case Event::EventType::DisplayTop5Scores:
					Graphical_interface.set_fianal_scores(*(std::vector<float>*)(EventHandler::EventQueue[i]->get_value()));
					break;
				default:
					std::cout << "Graphics function enum out of range" << std::endl;
					break;
				}
				EventHandler::EventQueue[i]->subsystems_involved[j] = Event::NoSubSystem;
			}
			else if (EventHandler::EventQueue[i]->subsystems_involved[j] > Event::Graphics ||
				EventHandler::EventQueue[i]->subsystems_involved[j] == Event::End) {
				break;
			}
		}
	}
}

void GraphicsSystem::Load_scene_models()
{
	std::cout << "...loading models..." << std::endl;
	irr::scene::IAnimatedMesh* mesh;
	irr::scene::IAnimatedMeshSceneNode* anim_node;
	irr::scene::IMesh* primitive_mesh;
	irr::scene::IMeshSceneNode* mesh_node;
	std::string assets_path = get_file_path() + "\\Assets\\";

	for (int i = 0; i < GameObject::GameObjects.size(); ++i) {
		mesh = nullptr;
		primitive_mesh = nullptr;
		anim_node = nullptr;
		mesh_node = nullptr;
		if (GameObject::GameObjects[i]->get_model_node()) {
			if (GameObject::GameObjects[i]->get_model_node()->get_model_mesh_path() != "") {
				mesh = smgr->getMesh((assets_path + GameObject::GameObjects[i]->get_model_node()->get_model_mesh_path()).c_str()); //try to load the mesh
			}
			else {
				if (GameObject::GameObjects[i]->get_primitive() == GameObject::Primitives::Empty) { //empty gameobject no model
					continue;
				}
			}
		}
		if (mesh){//we have a mesh/model is present
			Model_mesh new_mesh = Model_mesh(GameObject::GameObjects[i]->get_model_node()->get_model_mesh_path().c_str(), mesh); //we save the model location used in an array of strings, reddundant with irrlicht, but can store models used.
			meshes_in_scene.push_back(new_mesh);
			anim_node = smgr->addAnimatedMeshSceneNode(mesh); //we load it, here's an anim node, can be animated
			GameObject::GameObjects[i]->set_primitive(GameObject::Primitives::NoPrimitive);
		}
		else {//if the mesh is not present 
			if (GameObject::GameObjects[i]->get_primitive() == GameObject::Primitives::NoPrimitive) { //and there's no primitive
				std::cout << "Error with model loading" << std::endl; //the file is missing or some other error
				mesh = smgr->getMesh(missing_model_path.c_str()); //load an error model
				mesh_node = smgr->addMeshSceneNode(mesh); //create a new node (used in irrlicht as a model pointer)
				mesh_node->setMesh(mesh);
			}
			else if (GameObject::GameObjects[i]->get_primitive() != GameObject::Primitives::Empty) { //there's a primitive so no model
				switch (GameObject::GameObjects[i]->get_primitive()) //create a primitive based on primitive enum
				{
				case GameObject::Primitives::Sphere:
					primitive_mesh = geomtry_creator->createSphereMesh(1.0f);
					break;
				case GameObject::Primitives::Cube:
					primitive_mesh = geomtry_creator->createCubeMesh(irr::core::vector3df(2.0f));
					break;
				case GameObject::Primitives::Plane:
					primitive_mesh = geomtry_creator->createPlaneMesh(irr::core::dimension2df(irr::f32(2.0f), irr::f32(2.0f)));
					break;
				case GameObject::Primitives::Cylinder:
					primitive_mesh = geomtry_creator->createCylinderMesh(1.0f, 1.0f, 1);
					break;
				default:
					break;
				}
				mesh_node = smgr->addMeshSceneNode(primitive_mesh);
				mesh_node->setMesh(primitive_mesh);
			}
		}
		if (anim_node || mesh_node) { //for animating node
			if (!GameObject::GameObjects[i]->get_model_node()) {
				GameObject::GameObjects[i]->set_model_node(new ModelNode());
			}
			if (GameObject::GameObjects[i]->get_model_node()->get_texture_path() != "") { //if the texture is not empty
				if (anim_node) {
					anim_node->setMaterialTexture(0, driver->getTexture((assets_path + GameObject::GameObjects[i]->get_model_node()->get_texture_path()).c_str())); //load the texture
				}
				else {
					mesh_node->setMaterialTexture(0, driver->getTexture((assets_path + GameObject::GameObjects[i]->get_model_node()->get_texture_path()).c_str()));
				}
			}
			else {
				if (anim_node) {
					anim_node->setMaterialTexture(0, driver->getTexture(missing_texture_path.c_str())); //otherwise the missing texture;
				}
				else {
					mesh_node->setMaterialTexture(0, driver->getTexture(missing_texture_path.c_str()));
				}
			}
			if (anim_node) {
				anim_node->setMaterialFlag(irr::video::EMF_LIGHTING, false); //set the lighting
				anim_node->setPosition(irr::core::vector3df(Svec3::Convert_to_irr(GameObject::GameObjects[i]->get_position()))); //we get and set position
				GameObject::GameObjects[i]->get_model_node()->set_anim_node(anim_node);
			}
			else {
				mesh_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
				mesh_node->setPosition(irr::core::vector3df(Svec3::Convert_to_irr(GameObject::GameObjects[i]->get_position())));
				GameObject::GameObjects[i]->get_model_node()->set_mesh_node(mesh_node);
			}
		}
		else { //just in case some weird error happens.
			std::cout << "Weird Error when loading models" << std::endl;
		}
		if (GameObject::GameObjects[i]->get_model_node()) {
			GameObject::GameObjects[i]->get_model_node()->update_scale();
			GameObject::GameObjects[i]->get_model_node()->update_model(GameObject::GameObjects[i]->get_position(), GameObject::GameObjects[i]->get_rotation());
		}
		else { //just in case some weirder error happens.
			std::cout << "Even Weirder Error when loading models" << std::endl;
		}
	}
	std::cout << "...finished Loading models..." << std::endl;
}