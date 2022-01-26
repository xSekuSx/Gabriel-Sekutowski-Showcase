#pragma once

#include <irrlicht.h>
#include <IrrIMGUI/IrrIMGUI.h>
#include <IrrIMGUI/IncludeIrrlicht.h>
#include "SubSystem.h"

class GuiSubSystem : SubSystem
{
public:
	GuiSubSystem();
	~GuiSubSystem();
	void Init_Subsystem();
	void Update_Subsystem();
	void Process_events();
	//function overwtires SubSystems, due to being an instance of another subsystem - Graphics
	void Profiling();

	/*
	IMGUI requires these two functions to pass things between it and Irrlicht so the window interacts with the panels
	*/
	IrrIMGUI::CIMGUIEventReceiver& get_EventReceiver();
	void send_irrDevice(irr::IrrlichtDevice* dev);

	//sets the values of selected gameobject to corresponding display values
	void get_current_gameObject_values();
	//once the level editing object has been changed, call this function to kepp gui in sync
	void next_object(bool val);
	//once the level editing object has been updated, call this function to kepp gui in sync
	void update_object(bool val);

	//receive the final 5 scores and keep a record, change bool of display if names are present too
	void set_fianal_scores(std::vector<float> top5scores);
	//receive the final 5 names and keep a record, change bool of display if scores are present too
	void set_fianal_names(std::vector<std::string> top5names);

	//swtitch the current edited object
	void set_current_obj_id(int obj_id);

	//graphics will set the fps string when its is updated too
	void setFPSstring(std::string newFPS);
	//graphics will set the timer string when its is updated too
	void setTimer_string(std::string newTime);

	/*
	all the bellows static variables are the ones ImGui uses to keep track of the change in varaibles in the windows
	A second layer of variables has been provided to keep track of my Vector class Svec and keep switching easier to debug
	*/

	static std::string object_name, object_model_mesh, object_model_texture, object_sound_path;
	static float display_pos[3], display_model_pos[3], display_rigidBody_pos[3];
	static float display_rot[3], display_model_rot[3], display_rigidBody_rot[3];
	static float display_scale[3], display_model_scale[3], display_rigidBody_scale[3];
	static int object_primitive;
	static Svec3 object_posVec, object_rotVec, object_scaleVec, object_model_posVec, object_model_rotVec, object_model_scale, object_rigidBody_posVec, object_rigidBody_rotVec, object_rigidBody_scale;
	static float object_mass;
	static float* display_mass;
	static bool* object_sound_mono_stereo; //true if mono, false if stereo
private:
	irr::IrrlichtDevice* device; //irrlicht device, copy this from Graphics
	IrrIMGUI::IIMGUIHandle* imGUIdevice; //Imgui device, needs some parameters from irrlicht to function
	IrrIMGUI::CIMGUIEventReceiver EventReceiver; //event receiver so that imgui knows when a window is interacted 
	std::string FPS_string;
	std::string Timer_string;
	bool gui_next_object = false;
	bool gui_update_object = true;
	bool show_final_game_scores = false;
	int current_object_id; //the id or index of the current object being edited.

	std::vector<std::string> names; //top 5 names from website
	std::vector<float> scores; //top 5 scores from website

	std::string table = "";

	Clock::time_point GuiCurrentTime; //current time in gui
	Clock::time_point GuiOldTime; //time of gui start
	Clock::duration GuiDeltaTime; //change srtart - end of gui system
};