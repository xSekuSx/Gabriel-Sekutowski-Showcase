#pragma once
#include <string>
#include <vector>
#include <string>

#include <irrlicht.h>

#include "SubSystem.h"
#include "GameObject.h"
#include "Player.h"
#include "GuiSubSystem.h"

/*
an outdated but once maybe useful struct holding all used models
*/
struct Model_mesh {
	std::string location;
	irr::scene::IAnimatedMesh* mesh_pointer;
	Model_mesh() {
		location = "";
		mesh_pointer = nullptr;
	}
	Model_mesh(std::string mesh_location, irr::scene::IAnimatedMesh* mesh_node_ptr) {
		location = mesh_location;
		mesh_pointer = mesh_node_ptr;
	}
};

class GraphicsSystem : SubSystem{
public:
	GraphicsSystem();
	~GraphicsSystem();
	void Init_Subsystem(int width, int height, bool full_screen);
	void Update_Subsystem();
	void Create_window(bool full_screen);
	void Process_events();
	//lead the models in the scene once the gameObjects have been loaded into the engine 
	void Load_scene_models();
	void make_cursor_visible(bool on_off);
	//event response, moves the camera
	void move_camera(Event* event);
	//delete all model nodes and other irrlicht noces that are in the scene, including the cameras
	void delete_models();
	//adds two cameras, one for gameplay and other for level editing
	void add_camera();
	//add a model node to the currnet gameObject in level editing
	void add_model_node(Event* event);

	GuiSubSystem Graphical_interface;
protected:

	int HEIGHT, WIDTH; //width and height of the window
	irr::IrrlichtDevice* device; 
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::scene::IMesh* memr;
	irr::gui::IGUIEnvironment* guienv;
	irr::scene::ICameraSceneNode* irr_fps_camera; //this camera will hold irrlicht first person camera
	irr::scene::ICameraSceneNode* irr_maya_camera; //maya camera used in level editing
	irr::scene::ICameraSceneNode* irr_active_camera; //the current used camera, changes in level editing or running simulation or game

	const irr::scene::IGeometryCreator* geomtry_creator; //this allows me to create geomtry trough irrlicht
	std::vector<Model_mesh> meshes_in_scene;
	std::string missing_model_path; //path for a misssing model, an error model
	std::string missing_texture_path; //path for a missing texture

	irr::core::stringw FPS_display, Timer_display; //strings used by irrlicht gui
	std::string FPS_string, Timer_string;
	irr::gui::IGUIStaticText* FPSTextBox;
	irr::gui::IGUIStaticText* TimerTextBox;

	const float movement_multiplier = 0.25f; //movement of the camera
	bool window_was_active = true; //state of window last frame //used for pausing and resuming
};

