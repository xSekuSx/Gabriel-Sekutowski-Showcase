#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <enet\enet.h>
#include <vector>

const int NICK_SIZE = 3;

struct playerScore {
	char nick[NICK_SIZE];
	float score;
};

class Networking
{
public:
	//send a ticket to server, and set the values you get to top5 varaibles
	void communicate(float FinalTime, std::string nick);
	//destroy the valeus used by the networking
	void destroy();
	std::vector<std::string> top5_final_players; //vector that holds top 5 players
	std::vector<float> top5_final_scores; //vector that holds top 5 scores
private:
	float* HighScores;
	playerScore top5_sending[5];
	ENetAddress address;
	ENetHost* client;
	ENetPeer* peer;
	ENetEvent enetEvent;
	ENetPacket* dataPacket;
};

