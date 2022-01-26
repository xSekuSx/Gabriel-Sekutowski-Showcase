#include "SoundAgent.h"

SoundAgent::SoundAgent(Svec3 pos, Svec3 forward_vec, Svec3 vel, std::string sound_file_name,bool mono, bool listener)
{
	position = pos;
	forward = forward_vec;
	velocity = vel;
	sound_name = sound_file_name;
	mono_stereo = mono;
	listener_emmiter = listener;
}

SoundAgent::~SoundAgent()
{
	alDeleteSources(1, &Sound_source);
	alDeleteBuffers(1, &Sound_buffer);
}

void SoundAgent::set_position(Svec3 pos)
{
	position = pos;
}

void SoundAgent::set_forward(Svec3 forward_vec)
{
	forward = forward_vec;
}

void SoundAgent::set_velocity(Svec3 vel)
{
	velocity = vel;
}

Svec3 SoundAgent::get_position()
{
	return position;
}

Svec3 SoundAgent::get_forward()
{
	return forward;
}

Svec3 SoundAgent::get_velocity()
{
	return velocity;
}

std::string SoundAgent::get_sound_name()
{
	return sound_name;
}

ALuint& SoundAgent::get_sound_source()
{
	return Sound_source;
}

ALuint& SoundAgent::get_sound_buffer()
{
	return Sound_buffer;
}

ALint& SoundAgent::get_source_state()
{
	return source_state;
}

void SoundAgent::set_sound_name(std::string new_sound)
{
	sound_name = new_sound;
}
