/*
I have used this video totorial for the initial set up: https://www.youtube.com/watch?v=WvND0djMcfE&t=385s
Some simmilarities between the code in this repo and my initialise fonction can be founce since I have used it as basis for the subsytem https://github.com/mattstone22133/OpenAL_TestProject/blob/master/TestOpenAL/src/main.cpp
Some of his sample sound files can also be found in the sound folder
*/

#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <AudioFile.h>
#include "SubSystem.h"
#include "Player.h"

class SoundSystem : SubSystem
{
public:
	SoundSystem();
	~SoundSystem();
	void Init_Subsystem();
	void Update_Subsystem();
	void Process_events();
	//call after the gameobjects have been loaded, create buffers for sounds
	void load_agents();
	//a trigger to play a sound in the gameworld
	void play_sound();
	//play stereo sound, this would be the background music
	void play_theme();
	//plays a sound assosciated with the player
	void play_player_sound();
	//ceates a new sound agents and likds it with the right gameObject
	void add_sound_agent(Event* event);
private:
	ALCcontext* context;
	ALCdevice* device;
};

