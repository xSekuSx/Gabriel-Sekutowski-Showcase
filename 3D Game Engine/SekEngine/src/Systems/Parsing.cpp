#include "Parsing.h"

//https://github.com/nlohmann/json

std::string get_default_string(options option)
{
	std::ifstream i("Default.json");
	json Default = json::parse(i);
	switch (option)
	{
	case options::Save:
		return Default["save level"].get<std::string>();
	case options::User_name:
		return Default["user name"].get<std::string>();
	default:
		break;
	}
}

int get_default_int(options option)
{
	std::ifstream i("Default.json");
	json Default = json::parse(i);
	switch (option) {
	case options::ResolutionY:
		return Default["resolutionY"].get<int>();
	case options::ResolutionX:
		return Default["resolutionX"].get<int>();
	default:
		break;
	}
}

bool get_default_bool(options option)
{
	std::ifstream i("Default.json");
	json Default = json::parse(i);
	switch (option)
	{
	case options::Level_editing:
		return Default["level editing"].get<bool>();
	case options::Full_screen:
		return Default["full screen"].get<bool>();
	case options::Initialise_Debugging:
		return Default["debugging"].get<bool>();
	default:
		break;
	}
}

std::vector<std::string> get_default_vector(options option)
{
	std::ifstream i("Default.json");
	json Default = json::parse(i);

	std::vector<std::string> data_vector;

	switch (option)
	{
	case options::Levels_list:
		std::cout << Default["levels"].size() << std::endl;
		for (int i = 0; i < Default["levels"].size(); ++i) {
			data_vector.push_back(Default["levels"][i]);
		}
		return data_vector;
	default:
		break;
	}
}

void save_scene_to_file(std::string save_path) {
	save_path += ".json";
	json save_file;
	save_file["Number"] = GameObject::GameObjects.size();
	for (int i = 0; i < GameObject::GameObjects.size(); ++i){
		save_file["Objects"][i]["name"] = GameObject::GameObjects[i]->get_name();
		save_file["Objects"][i]["position"] = Svec3::get_vector(GameObject::GameObjects[i]->get_position());
		save_file["Objects"][i]["rotation"] = Svec3::get_vector(GameObject::GameObjects[i]->get_rotation());
		save_file["Objects"][i]["scale"] = Svec3::get_vector(GameObject::GameObjects[i]->get_scale());
		save_file["Objects"][i]["primitive"] = GameObject::GameObjects[i]->get_primitive();
		save_file["Objects"][i]["tag"] = GameObject::GameObjects[i]->get_tag();
		save_file["Objects"][i]["ModelNode"]["ModelNode"] = GameObject::GameObjects[i]->get_model_node() != nullptr;
		if (GameObject::GameObjects[i]->get_model_node() != nullptr) {
			save_file["Objects"][i]["ModelNode"]["model"] = GameObject::GameObjects[i]->get_model_node()->get_model_mesh_path();
			save_file["Objects"][i]["ModelNode"]["texture"] = GameObject::GameObjects[i]->get_model_node()->get_texture_path();
			save_file["Objects"][i]["ModelNode"]["offset"] = Svec3::get_vector(GameObject::GameObjects[i]->get_model_node()->get_position_offset());
			save_file["Objects"][i]["ModelNode"]["rotation"] = Svec3::get_vector(GameObject::GameObjects[i]->get_model_node()->get_rotational_offset());
			save_file["Objects"][i]["ModelNode"]["scale"] = Svec3::get_vector(GameObject::GameObjects[i]->get_model_node()->get_model_scale());
		}
		save_file["Objects"][i]["RigidBody"]["RigidBody"] = GameObject::GameObjects[i]->get_rigidbody() != nullptr;
		if (GameObject::GameObjects[i]->get_rigidbody() != nullptr) {
			save_file["Objects"][i]["RigidBody"]["transform origin"] = Svec3::get_vector(GameObject::GameObjects[i]->get_rigidbody()->get_transform_position() - GameObject::GameObjects[i]->get_position());//get difference first then store it
			save_file["Objects"][i]["RigidBody"]["transform matrix"][0] = Svec3::get_vector(GameObject::GameObjects[i]->get_rigidbody()->get_transform_matrix().row1);
			save_file["Objects"][i]["RigidBody"]["transform matrix"][1] = Svec3::get_vector(GameObject::GameObjects[i]->get_rigidbody()->get_transform_matrix().row2);
			save_file["Objects"][i]["RigidBody"]["transform matrix"][2] = Svec3::get_vector(GameObject::GameObjects[i]->get_rigidbody()->get_transform_matrix().row3);
			save_file["Objects"][i]["RigidBody"]["mass"] = GameObject::GameObjects[i]->get_rigidbody()->get_mass();
			save_file["Objects"][i]["RigidBody"]["collider scale"] = Svec3::get_vector(GameObject::GameObjects[i]->get_rigidbody()->get_collider_scale());
			save_file["Objects"][i]["RigidBody"]["collider shape"] = GameObject::GameObjects[i]->get_rigidbody()->get_collider_shape();
		}
		save_file["Objects"][i]["SoundAgent"]["SoundAgent"] = GameObject::GameObjects[i]->get_sound_agent() != nullptr;
		if (GameObject::GameObjects[i]->get_sound_agent() != nullptr) {
			save_file["Objects"][i]["SoundAgent"]["forward"] = Svec3::get_vector(GameObject::GameObjects[i]->get_sound_agent()->get_forward());
			save_file["Objects"][i]["SoundAgent"]["sound name"] = GameObject::GameObjects[i]->get_sound_agent()->get_sound_name();
			save_file["Objects"][i]["SoundAgent"]["mono"] = GameObject::GameObjects[i]->get_sound_agent()->mono_stereo;
			save_file["Objects"][i]["SoundAgent"]["listener"] = GameObject::GameObjects[i]->get_sound_agent()->listener_emmiter;
		}
	}

	save_file["Player"][0]["position"] = Svec3::get_vector(Player::Players[0]->get_position());
	save_file["Player"][0]["scale"] = Svec3::get_vector(Player::Players[0]->get_scale());
	save_file["Player"][0]["primitive"] = Player::Players[0]->get_primitive();
	save_file["Player"][0]["ModelNode"]["ModelNode"] = Player::Players[0]->get_model_node() != nullptr;
	if (Player::Players[0]->get_model_node() != nullptr) {
		save_file["Player"][0]["ModelNode"]["model"] = Player::Players[0]->get_model_node()->get_model_mesh_path();
		save_file["Player"][0]["ModelNode"]["texture"] = Player::Players[0]->get_model_node()->get_texture_path();
		save_file["Player"][0]["ModelNode"]["offset"] = Svec3::get_vector(Player::Players[0]->get_model_node()->get_position_offset());
		save_file["Player"][0]["ModelNode"]["rotation"] = Svec3::get_vector(Player::Players[0]->get_model_node()->get_rotational_offset());
		save_file["Player"][0]["ModelNode"]["scale"] = Svec3::get_vector(Player::Players[0]->get_model_node()->get_model_scale());
	}
	save_file["Player"][0]["RigidBody"]["RigidBody"] = Player::Players[0]->get_rigidbody() != nullptr;
	if (Player::Players[0]->get_rigidbody() != nullptr) {
		save_file["Player"][0]["RigidBody"]["transform origin"] = Svec3::get_vector(Player::Players[0]->get_rigidbody()->get_transform_position() - Player::Players[0]->get_position());//get difference first then store it
		save_file["Player"][0]["RigidBody"]["transform matrix"][0] = Svec3::get_vector(Player::Players[0]->get_rigidbody()->get_transform_matrix().row1);
		save_file["Player"][0]["RigidBody"]["transform matrix"][1] = Svec3::get_vector(Player::Players[0]->get_rigidbody()->get_transform_matrix().row2);
		save_file["Player"][0]["RigidBody"]["transform matrix"][2] = Svec3::get_vector(Player::Players[0]->get_rigidbody()->get_transform_matrix().row3);
		save_file["Player"][0]["RigidBody"]["mass"] = Player::Players[0]->get_rigidbody()->get_mass();
		save_file["Player"][0]["RigidBody"]["collider scale"] = Svec3::get_vector(Player::Players[0]->get_rigidbody()->get_collider_scale());
		save_file["Player"][0]["RigidBody"]["collider shape"] = Player::Players[0]->get_rigidbody()->get_collider_shape();
	}
	save_file["Player"][0]["SoundAgent"]["SoundAgent"] = Player::Players[0]->get_sound_agent() != nullptr;
	if (Player::Players[0]->get_sound_agent() != nullptr) {
		save_file["Player"][0]["SoundAgent"]["forward"] = Svec3::get_vector(Player::Players[0]->get_sound_agent()->get_forward());
		save_file["Player"][0]["SoundAgent"]["sound name"] = Player::Players[0]->get_sound_agent()->get_sound_name();
		save_file["Player"][0]["SoundAgent"]["mono"] = Player::Players[0]->get_sound_agent()->mono_stereo;
		save_file["Player"][0]["SoundAgent"]["listener"] = Player::Players[0]->get_sound_agent()->listener_emmiter;
	}

	std::ofstream o(save_path);
	o << std::setw(6) << save_file << std::endl;
}

void load_scene_from_file(std::string load_path)
{
	load_path += ".json";
	std::ifstream i(load_path);
	json load_file = json::parse(i);

	Player::Players[0]->set_position(Svec3(load_file["Player"][0]["position"][0], load_file["Player"][0]["position"][1], load_file["Player"][0]["position"][2]));
	Player::Players[0]->set_scale(Svec3(load_file["Player"][0]["scale"][0], load_file["Player"][0]["scale"][1], load_file["Player"][0]["scale"][2]));
	Player::Players[0]->set_primitive(load_file["Player"][0]["primitive"]);

	if (load_file["Player"][0]["RigidBody"]["RigidBody"]) {
		SekRigidBody* SekRB = new SekRigidBody(
			Player::Players[0]->get_position() + Svec3(load_file["Player"][0]["RigidBody"]["transform origin"][0], load_file["Player"][0]["RigidBody"]["transform origin"][1], load_file["Player"][0]["RigidBody"]["transform origin"][2]),
			Smatrix3(Svec3(load_file["Player"][0]["RigidBody"]["transform matrix"][0][0], load_file["Player"][0]["RigidBody"]["transform matrix"][0][1], load_file["Player"][0]["RigidBody"]["transform matrix"][0][2]),
				Svec3(load_file["Player"][0]["RigidBody"]["transform matrix"][1][0], load_file["Player"][0]["RigidBody"]["transform matrix"][1][1], load_file["Player"][0]["RigidBody"]["transform matrix"][1][2]),
				Svec3(load_file["Player"][0]["RigidBody"]["transform matrix"][2][0], load_file["Player"][0]["RigidBody"]["transform matrix"][2][1], load_file["Player"][0]["RigidBody"]["transform matrix"][2][2])),
			Svec3(load_file["Player"][0]["RigidBody"]["collider scale"][0], load_file["Player"][0]["RigidBody"]["collider scale"][1], load_file["Player"][0]["RigidBody"]["collider scale"][2]),
			Player::Players[0]->get_scale(),
			load_file["Player"][0]["RigidBody"]["collider shape"],
			load_file["Player"][0]["RigidBody"]["mass"]
		);
		Player::Players[0]->set_rigid_body(SekRB);
	}

	if (load_file["Player"][0]["ModelNode"]["ModelNode"]) {
		ModelNode* Mnode = new ModelNode(
			load_file["Player"][0]["ModelNode"]["model"],
			load_file["Player"][0]["ModelNode"]["texture"],
			Svec3(load_file["Player"][0]["ModelNode"]["offset"][0], load_file["Player"][0]["ModelNode"]["offset"][1], load_file["Player"][0]["ModelNode"]["offset"][2]),
			Svec3(load_file["Player"][0]["ModelNode"]["rotation"][0], load_file["Player"][0]["ModelNode"]["rotation"][1], load_file["Player"][0]["ModelNode"]["rotation"][2]),
			Svec3(load_file["Player"][0]["ModelNode"]["scale"][0], load_file["Player"][0]["ModelNode"]["scale"][1], load_file["Player"][0]["ModelNode"]["scale"][2]),
			Player::Players[0]->get_scale()
		);

		Player::Players[0]->set_model_node(Mnode);
	}

	if (load_file["Player"][0]["SoundAgent"]["SoundAgent"]) {
		SoundAgent* SA = new SoundAgent(
			Player::Players[0]->get_position(),
			Svec3(load_file["Player"][0]["SoundAgent"]["forward"][0], load_file["Player"][0]["SoundAgent"]["forward"][1], load_file["Player"][0]["SoundAgent"]["forward"][2]),
			Svec3(0),
			load_file["Player"][0]["SoundAgent"]["sound name"],
			load_file["Player"][0]["SoundAgent"]["mono"],
			load_file["Player"][0]["SoundAgent"]["listener"]
		);

		Player::Players[0]->set_sound_agent(SA);
	}

	int size = load_file["Number"].get<int>();
	for (int i = 0; i < size; ++i) {
		GameObject* new_go = new GameObject(
			load_file["Objects"][i]["name"],
			Svec3(load_file["Objects"][i]["position"][0], load_file["Objects"][i]["position"][1], load_file["Objects"][i]["position"][2]),
			Svec3(load_file["Objects"][i]["rotation"][0], load_file["Objects"][i]["rotation"][1], load_file["Objects"][i]["rotation"][2]),
			Svec3(load_file["Objects"][i]["scale"][0], load_file["Objects"][i]["scale"][1], load_file["Objects"][i]["scale"][2]),
			load_file["Objects"][i]["primitive"],
			load_file["Objects"][i]["tag"]
		);

		if (load_file["Objects"][i]["RigidBody"]["RigidBody"]) {
			SekRigidBody* SekRB =  new SekRigidBody(
				GameObject::GameObjects[GameObject::GameObjects.size() - 1]->get_position() + Svec3(load_file["Objects"][i]["RigidBody"]["transform origin"][0], load_file["Objects"][i]["RigidBody"]["transform origin"][1], load_file["Objects"][i]["RigidBody"]["transform origin"][2]),
				Smatrix3(Svec3(load_file["Objects"][i]["RigidBody"]["transform matrix"][0][0], load_file["Objects"][i]["RigidBody"]["transform matrix"][0][1], load_file["Objects"][i]["RigidBody"]["transform matrix"][0][2]),
					Svec3(load_file["Objects"][i]["RigidBody"]["transform matrix"][1][0], load_file["Objects"][i]["RigidBody"]["transform matrix"][1][1], load_file["Objects"][i]["RigidBody"]["transform matrix"][1][2]),
					Svec3(load_file["Objects"][i]["RigidBody"]["transform matrix"][2][0], load_file["Objects"][i]["RigidBody"]["transform matrix"][2][1], load_file["Objects"][i]["RigidBody"]["transform matrix"][2][2])),
				Svec3(load_file["Objects"][i]["RigidBody"]["collider scale"][0], load_file["Objects"][i]["RigidBody"]["collider scale"][1], load_file["Objects"][i]["RigidBody"]["collider scale"][2]),
				GameObject::GameObjects[GameObject::GameObjects.size() - 1]->get_scale(),
				load_file["Objects"][i]["RigidBody"]["collider shape"],
				load_file["Objects"][i]["RigidBody"]["mass"]
			);
			GameObject::GameObjects[::GameObject::GameObjects.size() - 1]->set_rigid_body(SekRB);
		}
		if (load_file["Objects"][i]["SoundAgent"]["SoundAgent"]) {
			SoundAgent* SA = new SoundAgent(
				GameObject::GameObjects[GameObject::GameObjects.size()-1]->get_position(),
				Svec3(load_file["Objects"][i]["SoundAgent"]["forward"][0], load_file["Objects"][i]["SoundAgent"]["forward"][1], load_file["Objects"][i]["SoundAgent"]["forward"][2]),
				Svec3(0),
				load_file["Objects"][i]["SoundAgent"]["sound name"],
				load_file["Objects"][i]["SoundAgent"]["mono"],
				load_file["Objects"][i]["SoundAgent"]["listener"]
			);

			GameObject::GameObjects[GameObject::GameObjects.size() - 1]->set_sound_agent(SA);
		}
		if (load_file["Objects"][i]["ModelNode"]["ModelNode"]) {
			ModelNode* Mnode = new ModelNode(
				load_file["Objects"][i]["ModelNode"]["model"],
				load_file["Objects"][i]["ModelNode"]["texture"],
				Svec3(load_file["Objects"][i]["ModelNode"]["offset"][0], load_file["Objects"][i]["ModelNode"]["offset"][1], load_file["Objects"][i]["ModelNode"]["offset"][2]),
				Svec3(load_file["Objects"][i]["ModelNode"]["rotation"][0], load_file["Objects"][i]["ModelNode"]["rotation"][1], load_file["Objects"][i]["ModelNode"]["rotation"][2]),
				Svec3(load_file["Objects"][i]["ModelNode"]["scale"][0], load_file["Objects"][i]["ModelNode"]["scale"][1], load_file["Objects"][i]["ModelNode"]["scale"][2]),
				GameObject::GameObjects[GameObject::GameObjects.size() - 1]->get_scale()
			);

			GameObject::GameObjects[GameObject::GameObjects.size() - 1]->set_model_node(Mnode);
		}
	}

	std::cout << "...Game Objects loaded..." << std::endl;
}

std::vector<std::string> Lua_load_file(lua_State* L, std::string file_name, const std::string& tab)
{
	std::string source =
		"function getElements(tab) "
		"s = \"\""
		"for k, v in pairs(_G[tab]) do "
		"    s = s..k..\"|\" "
		"    end "
		"return s "
		"end";

	/* We load the function using the loadstring function, then set up our
	default preamble. We then use getGlobal to access the getElements
	function, and pass in the name of the table we wish to explore (in this
	case, elementList, or 'tab'). The second lua_pcall executes the function
	we've loaded. */

	luaL_loadstring(L, source.c_str());
	lua_pcall(L, 0, 0, 0);
	lua_getglobal(L, "getElements");
	lua_pushstring(L, tab.c_str());
	lua_pcall(L, 1, 1, 0);

	/* The return from the function will be a single string, so we'll need to
	parse it.  This is why we added the separation character in the function
	code. We simply iterate through ans, populating a temp string, and push
	temp onto elements whenever the separation character is reached. */

	std::string ans = lua_tostring(L, -1);
	std::vector<std::string> elements;
	std::string temp = "";

	for (unsigned int i = 0; i < ans.size(); i++) {
		if (ans.at(i) != '|') {
			temp += ans.at(i);
		}
		else {
			elements.push_back(temp);
			temp = "";
		}
	}

	if (ans.size() == 0) {
		std::cout << "File not present" << std::endl;
	}
	else {
		std::cout << "File openened, loading" << std::endl;
	}

	/* We clean up after ourselves as best we can. */

	int n = lua_gettop(L);
	lua_pop(L, 1);

	/* Lastly, return the list of elements. With thanks to Elias Daler.*/

	return elements;
}

std::string get_file_path()
{
	std::filesystem::path path = std::filesystem::current_path();
	return path.string();
}
