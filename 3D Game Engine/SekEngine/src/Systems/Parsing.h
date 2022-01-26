#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "GameObject.h"
#include "Player.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <filesystem>
#include <fstream>
#include <LuaBridge\LuaBridge.h>
using namespace luabridge;

//class of the options you can choose, should be similar to the headings in default.json
enum class options {No_option, Save, Level_editing, ResolutionX, ResolutionY, Full_screen, User_name, Initialise_Debugging, Levels_list};

//get a string from default.json
std::string get_default_string(options option);
//get a int from default.json
int get_default_int(options option);
//get a bool from default.json
bool get_default_bool(options option);
//get a list from default.json
std::vector<std::string> get_default_vector(options option);

//save the current scene/level into a json file that can be found Default.json
void save_scene_to_file(std::string save_path);
//load the contents of the filepath stated and load its gameObjetcs
void load_scene_from_file(std::string load_path);
std::vector<std::string> Lua_load_file(lua_State* state, std::string file_name, const std::string& table);
//get the path of the current executable
std::string get_file_path();