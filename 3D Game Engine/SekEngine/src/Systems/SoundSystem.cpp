/*
I have used this video totorial for the initial set up: https://www.youtube.com/watch?v=WvND0djMcfE&t=385s
Some simmilarities between the code in this repo and my initialise fonction can be founce since I have used it as basis for the subsytem https://github.com/mattstone22133/OpenAL_TestProject/blob/master/TestOpenAL/src/main.cpp
*/

#include "SoundSystem.h"

//OpenAL Error Check
#define OpenAL_ErrorCheck(MSG)\
{\
	ALenum error = alGetError();\
	if (error != AL_NO_ERROR)\
	{\
		std::cerr << "OpenAL error : " << error << "with call for " << #MSG << std::endl;\
	}\
}	

#define alec(FUNCTION_CALL)\
FUNCTION_CALL;\
OpenAL_ErrorCheck(FUNCTION_CALL)

auto getALSoundFormat = [](AudioFile<float>& audioFile)
{
	int bitDepth = audioFile.getBitDepth();
	if (bitDepth == 16)
		return audioFile.isStereo() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	else if (bitDepth == 8)
		return audioFile.isStereo() ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	else
		return -1;
};

SoundSystem::SoundSystem()
{
}

SoundSystem::~SoundSystem()
{ 
	alec(alcMakeContextCurrent(nullptr));
	alec(alcDestroyContext(context));
}

void SoundSystem::Init_Subsystem()
{
	std::cout << "Initialising Graphics" << std::endl;
	SubSystem_name = "Sound";

	//find out the default audio device
	const ALCchar* defaultDeviceString = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	device = alcOpenDevice(defaultDeviceString);
	if (!device){
		std::cerr << "failed to find device" << std::endl;
		return;
	}
	std::cout << "OpenAL device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
	OpenAL_ErrorCheck(device);

	//create an OpenAL audio context from the device;
	context = alcCreateContext(device, nullptr);
	OpenAL_ErrorCheck(context);

	//active this context so that OpenAL state modifications are applied to the context
	if (!alcMakeContextCurrent(context)){
		std::cerr << "failed to make context active" << std::endl;
		return;
	}

	//create a listener in 3d space
	alec(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
	alec(alListener3f(AL_VELOCITY, 0, 0, 0));
	ALfloat frontAndUpVectors[] = {
		/* front*/0.f, 0.f, 1.f,
		/* up */ 0.f, 1.f, 0.f
	};
	alec(alListenerfv(AL_ORIENTATION, frontAndUpVectors));
}

void SoundSystem::Update_Subsystem()
{
	Svec3 pos = Player::Players[0]->get_position();
	Svec3 vel = Svec3(0);
	if (Player::Players[0]->get_rigidbody()) {
		Svec3 vel = Player::Players[0]->get_rigidbody()->get_velocity();
	}
	Svec3 forward = Player::Players[0]->get_forward_vector();
	alec(alListener3f(AL_POSITION, -pos.x, pos.y, pos.z)); //axis are fucked
	alec(alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z));
	ALfloat frontAndUpVectors[] = {
		forward.x, forward.y, forward.z,
		0.f, 1.f, 0.f //up vector
	};
	alec(alListenerfv(AL_ORIENTATION, frontAndUpVectors));

	SoundAgent* SA;
	Svec3 vec3;
	for (int i = 0; i < GameObject::SoundGameObjects.size(); ++i) {
		SA = GameObject::SoundGameObjects[i]->get_sound_agent();
		if (SA->mono_stereo) {
			vec3 = GameObject::SoundGameObjects[i]->get_position();
			alec(alSource3f(SA->get_sound_source(), AL_POSITION, vec3.x, vec3.y, vec3.z));
		}
	}

	play_theme();
	Process_events();
	Profiling();
}

void SoundSystem::play_theme()
{
	SoundAgent* SA;
	for (int i = 0; i < GameObject::SoundGameObjects.size(); ++i) {
		if (GameObject::SoundGameObjects[i]->get_sound_agent()) {
			SA = GameObject::SoundGameObjects[i]->get_sound_agent();
			if (!SA->mono_stereo) {
				alec(alGetSourcei(SA->get_sound_source(), AL_SOURCE_STATE, &SA->get_source_state()));
				if (SA->get_source_state() != AL_PLAYING)
				{
					alec(alSourcePlay(SA->get_sound_source()));
				}
			}
		}
	}
}

void SoundSystem::play_player_sound()
{
	if (SubSystem::running_sim) {
		SoundAgent* SA;
		for (int i = 0; i < Player::Players.size(); ++i) {
			if (Player::Players[i]->get_sound_agent()) {
				SA = Player::Players[i]->get_sound_agent();
				alec(alGetSourcei(SA->get_sound_source(), AL_SOURCE_STATE, &SA->get_source_state()));
				if (SA->get_source_state() != AL_PLAYING)
				{
					alec(alSourcePlay(SA->get_sound_source()));
				}
				//playing the bottom will just replay the sound
				//alec(alSourcePlay(SA->get_sound_source()));
			}
		}
	}
}

void SoundSystem::add_sound_agent(Event* event)
{
	//needs to create buffers, no time to implement now
	//int id = *(int*)event->get_value(); //gives some errors, to be implemented
	//GameObject::GameObjects[id]->set_sound_agent(new SoundAgent());
}

void SoundSystem::Process_events()
{
	for (int i = 0; i < EventHandler::EventQueue.size(); ++i) {
		for (int j = 0; Event::Subsystems_size; ++j) {
			if (EventHandler::EventQueue[i]->subsystems_involved[j] == Event::Sound) {
				switch (EventHandler::EventQueue[i]->type)
				{
				case Event::EventType::Load:
					load_agents();
					break;
				case Event::EventType::Trigger:
					play_sound();
					break;
				case Event::EventType::Shoot:
					play_player_sound();
					break;
				case Event::EventType::Add_sound_agent:
					add_sound_agent(EventHandler::EventQueue[i]);
					break;
				default:
					std::cout << "Sound System function enum out of range" << std::endl;
					break;
				}
				EventHandler::EventQueue[i]->subsystems_involved[j] = Event::NoSubSystem;
			}
			else if (EventHandler::EventQueue[i]->subsystems_involved[j] > Event::Sound ||
				EventHandler::EventQueue[i]->subsystems_involved[j] == Event::End) {
				break;
			}
		}
	}
}

void SoundSystem::play_sound()
{
	SoundAgent* SA;
	for (int i = 1; i < GameObject::SoundGameObjects.size(); ++i) {
		if (GameObject::SoundGameObjects[i]->get_sound_agent()) {
			SA = GameObject::SoundGameObjects[i]->get_sound_agent();
			alec(alGetSourcei(SA->get_sound_source(), AL_SOURCE_STATE, &SA->get_source_state()));
			if (SA->get_source_state() != AL_PLAYING)
			{
				alec(alSourcePlay(SA->get_sound_source()));
			}
			//playing the bottom will just replay the sound
			//alec(alSourcePlay(SA->get_sound_source()));
		}
	}
}

void SoundSystem::load_agents()
{
	std::cout << "...loading sounds..." << std::endl;
	std::vector<uint8_t> PCMData;
	AudioFile<float> SoundFile;
	Svec3 vec3;
	SoundAgent* SA;

	for (int i = 0; i < GameObject::SoundGameObjects.size(); ++i) {
		SA = GameObject::SoundGameObjects[i]->get_sound_agent();
		if (!SA->listener_emmiter) {
			if (!SoundFile.load(get_file_path() + "\\Assets\\Sounds\\" + SA->get_sound_name()))
			{
				std::cerr << "failed to load test mono sound" << std::endl;
				GameObject::SoundGameObjects[i]->set_sound_agent(nullptr);
				continue;
			}
			SoundFile.writePCMToBuffer(PCMData);
			alec(alGenBuffers(1, &SA->get_sound_buffer()));
			alec(alBufferData(SA->get_sound_buffer(), getALSoundFormat(SoundFile), PCMData.data(), PCMData.size(), SoundFile.getSampleRate()));
			alec(alGenSources(1, &SA->get_sound_source()));
			alec(alSourcef(SA->get_sound_source(), AL_PITCH, 1.f));
			alec(alSourcef(SA->get_sound_source(), AL_GAIN, 1.f));
			alec(alSourcei(SA->get_sound_source(), AL_LOOPING, AL_FALSE));
			alec(alSourcei(SA->get_sound_source(), AL_BUFFER, SA->get_sound_buffer()));
			if (SA->mono_stereo) {
				vec3 = SA->get_position();
				alec(alSource3f(SA->get_sound_source(), AL_POSITION, -vec3.x, vec3.y, vec3.z)); //axis are fucked
				vec3 = SA->get_velocity();
				alec(alSource3f(SA->get_sound_source(), AL_VELOCITY, -vec3.x, vec3.y, vec3.z));
			}
		}
	}

	std::cout << "...finished loading sounds..." << std::endl;
	message_finished_loading();
}