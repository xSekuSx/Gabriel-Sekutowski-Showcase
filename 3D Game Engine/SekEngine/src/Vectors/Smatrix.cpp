#include "Smatrix.h"

Smatrix3::~Smatrix3()
{
}

Smatrix3::Smatrix3(Svec3 first_row, Svec3 second_row, Svec3 third_row)
{
	row1 = first_row;
	row2 = second_row;
	row3 = third_row;
}