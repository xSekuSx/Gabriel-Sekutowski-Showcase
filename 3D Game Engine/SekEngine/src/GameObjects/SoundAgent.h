#pragma once
#include "Svec.h"
#include <AL/al.h>

class SoundAgent
{public:
	SoundAgent(Svec3 pos = Svec3(0), Svec3 forward_vec = Svec3(0, 0, 1), Svec3 vel = Svec3(0), std::string sound_file_name = "", bool mono = true, bool listener = false);
	~SoundAgent();
	void set_position(Svec3 pos);
	void set_forward(Svec3 forward_vec);
	void set_velocity(Svec3 vel);

	Svec3 get_position();
	Svec3 get_forward();
	Svec3 get_velocity();
	std::string get_sound_name();
	ALuint& get_sound_source();
	ALuint& get_sound_buffer();
	ALint& get_source_state();

	void set_sound_name(std::string new_sound);

	bool mono_stereo; //true if mono
	bool listener_emmiter; //true listener
private:
	Svec3 position;
	Svec3 forward;
	Svec3 velocity;
	std::string sound_name;

	ALuint Sound_source;
	ALuint Sound_buffer;
	ALint source_state;
};

