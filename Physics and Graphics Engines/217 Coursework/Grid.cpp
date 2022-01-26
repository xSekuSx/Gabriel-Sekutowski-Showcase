#include "Grid.h"

Grid::Grid()
{
}

Grid::~Grid()
{
}

Grid::Grid(int bin_size, int level)
{
	if (bin_size == 0);
	{
		std::cout << "Invalid cell size; division by 0" << std::endl;
	}

	size = bin_size;
	number_bins = pow(level * 2, 2);
	order = level;

	/*
	--	|  -+
		|
	----|----
		|
	+-	|  ++
	*/

	for (int i = -level; i < level + 1; ++i) {
		if (i == 0) {
			continue;
		}
		for (int j = -level; j < level + 1; ++j) {
			if (j == 0) {
				continue;
			}
			Bin new_bin = Bin();
			new_bin.x = i;
			new_bin.z = j;
			grid.insert(std::pair<Bin, std::vector<GameObject*>>(new_bin, std::vector<GameObject*>()));
		}
	}
}

int round_ext(float num) {
	if (num < 0) {
		return floor(num);
	}
	return ceil(num);
}

void Grid::Add_object(GameObject* entity)
{
	Collider* temp_col = entity->Get_collider();
	vec3 upper = entity->Get_position() - temp_col->diagonal;
	vec3 lower = upper + 2.0f * temp_col->diagonal;
	//find the bins it should be in
	Bin upper_bin, lower_bin;
	upper_bin.z = round_ext(upper.z / size);
	lower_bin.z = round_ext(lower.z / size);
	upper_bin.x = round_ext(upper.x / size);
	lower_bin.x = round_ext(lower.x / size);
	for (Bin b : {upper_bin, lower_bin}) {
		if (abs(b.x) > order) {
			if (b.x > 0) {
				b.x = order;
			}
			else {
				b.x = -order;
			}
		}
		if (abs(b.z) > order) {
			if (b.z > 0) {
				b.z = order;
			}
			else {
				b.z = -order;
			}
		}
	}
	for (int i = upper_bin.x; i < lower_bin.x; ++i) {
		if (i == 0) {
			continue;
		}
		for (int j = upper_bin.z; j < lower_bin.z; ++j) {
			if (j == 0) {
				continue;
			}
			Bin temp_bin = Bin();
			temp_bin.x = i;
			temp_bin.z = j;
			grid[temp_bin].push_back(entity);
			entity->Get_collider()->bins.push_back(temp_bin);
		}
	}
}

void Grid::Check(GameObject* entity)
{
	Collider* temp_col = entity->Get_collider();
	vec3 upper = entity->Get_position() - temp_col->diagonal;
	vec3 lower = upper + 2.0f * temp_col->diagonal;
	//find the bins it should be in
	Bin upper_bin , lower_bin;
	upper_bin.z = round_ext(upper.z / size);
	lower_bin.z = round_ext(lower.z / size);
	upper_bin.x = round_ext(upper.x / size);
	lower_bin.x = round_ext(lower.x / size);
	for (Bin b : {upper_bin, lower_bin}) {
		if (abs(b.x) > order) {
			if (b.x > 0) {
				b.x = order;
			}
			else {
				b.x = -order;
			}
		}
		if (abs(b.z) > order) {
			if (b.z > 0) {
				b.z = order;
			}
			else {
				b.z = -order;
			}
		}
	}

	//remove bins it is in
	for (int i = 0; i < temp_col->bins.size(); ++i) {
		remove(grid[temp_col->bins[i]].begin(), grid[temp_col->bins[i]].end(), entity);
	}
	entity->Get_collider()->bins.clear();

	//add it to the bins and connect ids
	for (int i = upper_bin.z; i < lower_bin.z + 1; ++i) {
		if (i == 0) {
			continue;
		}
		for (int j = upper_bin.x; j < lower_bin.x + 1; ++j) {
			if (j == 0) {
				continue;
			}
			Bin temp_bin = Bin();
			temp_bin.z = i;
			temp_bin.x = j;
			entity->Get_collider()->bins.push_back(temp_bin);
			grid[temp_bin].push_back(entity);
		}
	}
}