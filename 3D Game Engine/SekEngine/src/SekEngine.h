#pragma once
#include "PhysicsSystem.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "Networking.h"
#include "IncludeAllScripts.h"
#include "GameObject.h"
#include "Player.h"

namespace SekEngine {
	void Initialise_subsystems();
	void Update_loop();
	void Deactivate_subsytems();

	//should be called every frame, calculates DeltaTime
	void Calculate_delta_time();
	//check for corresponding event queue events
	void Process_events();
	//delete all the game objects freeing up space
	void Delete_game_objects();
	//loads from a corresponding json file
	void Load_scene_gameobjects(Event* event);
	//Saves the current level in level editor onto the one selected in Default.json
	void Save_level();
	//communicate with server and fetch scores and initiate display scores sequence
	void DisplayScores();
	//loads the next level in level_sequence and removes the first one
	void Load_new_level();
	//adds a new gameObject
	void Add_new_gameObject();
	//moves the current gameObject in level editor
	void Move_object(Event* event);
	//changes the current gameObject edited
	void Change_current_object_id(Event* event);
	//saves the parameters onto the actual gameObject NOT file
	void Save_current_object();

	int level_editing_model_selected = 0; //when in level editing, this int holds the id of the object that is being edited
	float move_object_multiplier = 1.0f; //how many units does an object move on edit
	bool axis_forward_up = false; //when false object will move forward, on true, up //also will rotate if up
	bool next_object = false; //make this true once you change the selected object int
	bool update_object = true; //make this true once you've made changes to selected object

	bool running = true; //keeps the main game loop running
	double accumulated_DeltaTime = 1001; //deltatime accumulation, keeps track of milliseconds since last called, used to update every second
	bool game_paused = false; //if window is out of focus this will be true
	std::vector<std::string> levels_sequence; //list of levels to be loaded
	//Clock::time_point StartTime;
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