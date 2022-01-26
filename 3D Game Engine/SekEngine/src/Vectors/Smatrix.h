#pragma once
#include "Svec.h"
class Smatrix3
{
public:
	~Smatrix3();
	Smatrix3(Svec3 first_row = Svec3(1, 0, 0), Svec3 second_row = Svec3(0, 1, 0), Svec3 third_row = Svec3(0, 0, 1));
	Svec3 row1, row2, row3;
private:
};

