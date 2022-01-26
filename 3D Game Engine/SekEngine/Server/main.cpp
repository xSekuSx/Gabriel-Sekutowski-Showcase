#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <enet/enet.h>

using namespace std;

const int NICK_SIZE = 3;

struct playerScore{
	char nick[NICK_SIZE];
	float score;
};

playerScore ImportedScores[5];

void main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Server Window");
	sf::Event e;


	sf::RectangleShape background;
	background.setSize(sf::Vector2f(800, 600));
	background.setPosition(0.0f, 0.0f);
	sf::Text text;

	std::string score_string;
	std::string nick;

	// set the string to display
	text.setString(score_string);
	sf::Font MyFont;
	MyFont.loadFromFile("Roboto-Black.ttf");
	text.setFont(MyFont);
	text.setPosition(300, 150);
	text.setCharacterSize(36);

	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("Textures/background.png"))
	{
		cout << "Texture did not load!" << "\n";
	}

	background.setTexture(&backgroundTexture);


	if (enet_initialize() != 0)
	{
		cout << "Enet failed to initialise!" << "\n\n";
	}

	ENetAddress address;
	ENetHost* server;
	ENetEvent enetEvent;

	ENetPacket* dataPacket;

	address.host = ENET_HOST_ANY;
	address.port = 1234;

	fstream site; //variable for file
	string text_line; //for each line of the file
	char current_letter;
	string current_line;
	string p_content;
	float loaded_scores[5]; 
	float* ImportedScoresPointer = new float[5];
	int i = 0;
	bool update_scores = false;
	bool now_name = true;

	playerScore top5players[5];

	site.open("HighScore.html");

	while (getline(site, text_line)) { //this while loop goes trough the entirety of the site
		current_line = "";
		for (char& c : text_line) {
			if (current_line == "<p>") {
				if (c == '<') {
					if (now_name) {
						for (int j = 0; j < NICK_SIZE; ++j) {
							top5players[i].nick[j] = p_content[j];
						}
						score_string += p_content + ": ";
						p_content = "";
						now_name = !now_name;
						break;
					}
					loaded_scores[i] = stof(p_content.c_str());
					top5players[i].score = loaded_scores[i];
					score_string += p_content + "\n";
					p_content = "";
					++i;
					now_name = !now_name;
					break;
				}
				p_content += c;
			}
			else {
				if (c == ' ') {
					continue;
				}
				current_line += c;
			}
		}
	}

	site.close();

	text.setString(score_string);

	for (int i = 0; i < 5; ++i) {
		std::cout << loaded_scores[i] << std::endl;
	}

	/* Now we create the server. We pass in the pointer to the address variable,
	the maximum number of clients we intend to support, the maximum number of
	channels to be used (in this case 2 channels, 0 and 1), and caps for
	upstream and downstream bandwidth (0 is uncapped). */

	server = enet_host_create(&address, 32, 2, 0, 0);

	/* Now we check server initialisation, before assigning some starting values
	to our position variable.*/

	if (server == NULL)
	{
		cout << "Server failed to initialise!" << "\n\n";
	}

	float temp = 0;

	p_content  = "";

	while (window.isOpen())
	{
		window.pollEvent(e);

		if (e.type == sf::Event::Closed) {
			window.close();
		}

		while (enet_host_service(server, &enetEvent, 0) > 0)
		{
			switch (enetEvent.type)
			{
			case ENET_EVENT_TYPE_CONNECT:

				cout << "A client connected from address " << enetEvent.peer->address.host << ":" << enetEvent.peer->address.port << ".\n";

				enetEvent.peer->data = (void*)"This is a client";

				dataPacket = enet_packet_create(&top5players, sizeof(playerScore) * 5, ENET_PACKET_FLAG_RELIABLE);
				enet_host_broadcast(server, 0, dataPacket);

				break;
			case ENET_EVENT_TYPE_RECEIVE:
				cout << "Packet received!\n";
				memcpy(ImportedScores, enetEvent.packet->data, enetEvent.packet->dataLength);
				for (int i = 0; i < 5; i++) {
					std::cout << ImportedScores[i].score << std::endl;
					std::cout << ImportedScores[i].nick << std::endl;
				}
				update_scores = true;
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				cout << "The client from address " << enetEvent.peer->address.host << ":" << enetEvent.peer->address.port << " disconnected \n";

				enetEvent.peer->data = NULL;

				break;
			}
		}

		if (update_scores) {
			update_scores = false;
			for (int i = 0; i < 5; i++) {
				if (loaded_scores[i] != ImportedScores[i].score) {
					update_scores = true;
					break;
				}
			}
			if (update_scores) {
				update_scores = false;

				std::string site_copy = "";

				site.open("HighScore.html"); //open

				int index = 0;

				score_string = "";
				p_content  = "";

				while (getline(site, text_line)) { //this while loop goes trough the entirety of the text needed
					current_line = "";
					p_content = "";
					for (char& c : text_line) {
						if (p_content == "<p>") {
							if (now_name) {
								current_line += std::string(ImportedScores[index].nick) + "</p>";
								score_string += std::string(ImportedScores[index].nick) + "\n";
								now_name = !now_name;
								break;
							}
							current_line += to_string(ImportedScores[index].score) + "</p>";
							score_string += to_string(ImportedScores[index].score) + "\n";
							index++;
							now_name = !now_name;
							break;
						}
						else {
							current_line += c;
							if (c != ' ') {
								p_content += c;
							}
						}
					}
					site_copy += current_line + "\n";
				}

				text.setString(score_string);
				site.close();

				ofstream siteOverWrite;
				siteOverWrite.open("HighScore.html");
				siteOverWrite << site_copy;
				siteOverWrite.close();
			}
		}

		window.clear(sf::Color::Blue);
		window.draw(background);
		window.draw(text);
		window.display();
	}

	/* We destroy the ENet Host instance, and deinitialize the library on shutdown. */

	enet_host_destroy(server);
	atexit(enet_deinitialize);
}