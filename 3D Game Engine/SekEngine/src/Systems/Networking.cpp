#include "Networking.h"

using namespace std;

void Networking::communicate(float FinalTime, std::string nick)
{

	if (enet_initialize() != 0)
	{
		cout << "Enet failed to initialise!" << "\n\n";
	}

	client = enet_host_create(NULL, 1, 2, 0, 0);

	if (client == NULL)
	{
		cout << "Client failed to initialise!" << "\n\n";
	}

	enet_address_set_host(&address, "localhost");
	address.port = 1234;

	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) {
		cout << "No available peers for initializing an ENet connection.\n";
	}

	HighScores = new float[5];
	playerScore* top5_received = new playerScore[5];
	float ranked_time = FinalTime;
	char ranked_nick[NICK_SIZE];

	for (int k = 0; k < NICK_SIZE; ++k) {
		ranked_nick[k] = nick[k];
	}

	float temp_time;
	char temp_nick[NICK_SIZE];
	float score;
	std::vector <float> scores_list;

	std::cout << "Waitng for Server to respond..." << std::endl;

	while (scores_list.size() == 0) {

		while (enet_host_service(client, &enetEvent, 0) > 0)
		{
			switch (enetEvent.type) {

			case ENET_EVENT_TYPE_RECEIVE:
				std::cout << "...it respoded, Yay!" << std::endl;
				cout << "Packet received!\n";
				memcpy(top5_received, enetEvent.packet->data, enetEvent.packet->dataLength);
				for (int i = 0; i < 5; i++) {
					if (ranked_time < top5_received[i].score) {
						temp_time = top5_received[i].score;
						top5_received[i].score = ranked_time;
						ranked_time = temp_time;

						for (int k = 0; k < NICK_SIZE; k++) {
							temp_nick[k] = top5_received[i].nick[k];
							top5_received[i].nick[k] = ranked_nick[k];
							ranked_nick[k] = temp_nick[k];
						}
					}
					top5_sending[i].score = top5_received[i].score;
					for (int k = 0; k < NICK_SIZE; k++) {
						temp_nick[k] = top5_received[i].nick[k];
						top5_received[i].nick[k] = top5_sending[i].nick[k];
						top5_sending[i].nick[k] = temp_nick[k];
					}
					std::cout << top5_sending[i].nick << std::endl;
					std::cout << top5_sending[i].score << std::endl;
					scores_list.push_back(top5_sending[i].score);
				}
				enetEvent.peer->data = (void*)"This is a server";
				std::cout << "Sending highscores back" << std::endl;
				dataPacket = enet_packet_create(&top5_sending, sizeof(playerScore) * 5, ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer, 0, dataPacket);
				break;
			}
		}
	}

	for (int i = 0; i < 5; ++i) {
		top5_final_players.push_back(std::string(top5_sending[i].nick));
		top5_final_scores.push_back(top5_sending[i].score);
	}
	return;
}

void Networking::destroy()
{
	if (peer != NULL) {
		enet_peer_disconnect_now(peer, 0);
		std::cout << "disconnected" << std::endl;
	}
	enet_host_destroy(client);
	atexit(enet_deinitialize);
	delete HighScores;
}
