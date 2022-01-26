#pragma once
#include <iostream>
#include <map>
#include "GameObject.h"
#include <string>

class Grid
{
private:

public:
	int number_bins, order;
	std::map<Bin, std::vector<GameObject*>> grid;
	float size;
	Grid();
	~Grid();
	Grid(int cell_size = 10, int level = 1);
	void Add_object(GameObject* entity);
	void Check(GameObject*);
};