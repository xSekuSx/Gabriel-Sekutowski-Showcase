#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>
#include <map>
using namespace std;

//global variables, columns is a vector of the columns on the board, three other variables are used to store the solutions as strings in a 2D vector
vector<char> columns = { 'a','b','c','d','e','f','g','h' };
string current_coposition;
vector<string> current_solution;
vector<vector<string>> solutions;

void add_queen(map<char, vector<int>>& chessBoard, string position) {
	/*
	function adds a queen at position that is passed as parameter
	position should be in chess form, from a1 to h8,
	the method converts those into a char and an int to access the map
	and sets all the positions the queen can go as 1s
	*/
	char column = position[0];
	int row = position[1] - '0' - 1; // this is the same as (int)character - 48 since this is basically ascii conversion and '0' = 48 in ascii
	for (int i = 0; i < 8; i++) {
		chessBoard[column][i] = 1;
	}//all the vertical
	for (int i = 0; i < 8; i++) {
		chessBoard[columns[i]][row] = 1;
	}// all the horizontal
	for (int i = 0; i < 4; i++) {
		auto it = find(columns.begin(), columns.end(), column); //finding the index of the column
		int index = distance(columns.begin(), it); //find the position of the letter in the columns
		int change = pow(-1, i);//variable so it switches between going up and down
		for (int j = row + change; j < 8 && j >= 0; j += change) {
			if (i < 2) {
				index++;
			}
			else {
				index--;
			}
			if (index > 7 || index < 0) {
				break;
			}
			chessBoard[columns[index]][j] = 1;
		}
	}//all diagonal ways
}

void display_chessboard(map<char, vector<int>>& chessBoard) { //used for debugging and code visualization
	/*cout << "\ta\tb\tc\td\te\tf\tg\th" << endl << endl; //this one shows the chessboard with heading from 0 to 7
	for (int i = 7; i >= 0; i--) {
		cout << i << "\t";
		for (int j = 0; j < 8; j++) {
			cout << chessBoard[columns[j]][i]<< "\t";
		}
		cout << endl;
	}*/
	cout << endl << "actual chessboard" << endl; //this one shows the chessboard with heading from 1 to 8
	cout << "\ta\tb\tc\td\te\tf\tg\th" << endl << endl;
	for (int i = 7; i >= 0; i--) {
		cout << i + 1 << "\t";
		for (int j = 0; j < 8; j++) {
			cout << chessBoard[columns[j]][i] << "\t";
		}
		cout << endl;
	}
}

string find_postions(map<char, vector<int>>chessBoard, int current_column) { //recursive function
	/*
	the function takes in the chessboard and the current column it is supposed to look on for spaces as parameters
	when it tries to place a queen it access the global variables and when it finds the solution
	*/
	//display_chessboard(chessBoard);
	//base case, if the column is 'h' (last column) and there are possible places to place the queen(there's a 0 on the map) then the solution is found
	if (find(chessBoard[columns[current_column]].begin(), chessBoard[columns[current_column]].end(), 0) != chessBoard[columns[current_column]].end()) { // check if there's a 0 in this column
		if (columns[current_column] == 'h') { // if its the last column
			auto it = find(chessBoard[columns[current_column]].begin(), chessBoard[columns[current_column]].end(), 0); //finding the index of the column
			int index = distance(chessBoard[columns[current_column]].begin(), it);
			current_coposition = columns[current_column] + to_string(index + 1);
			current_solution.push_back(current_coposition);
			solutions.push_back(current_solution);
			current_solution.pop_back();
			return "fail";
		}
	}
	else { //if there are no free spaces then last move was incorrect
		return "fail";
	}
	map<char, vector<int>>temp_chessBoard = chessBoard; //create a coppy of the chessboard, instead of removing last queen, I overwrite the board with how it was before placing her
	for (int i = 0; i < 8; i++) { //check all 8 positions
		if (chessBoard[columns[current_column]][i] != 1) { //if are free try to place a queen
			add_queen(chessBoard, columns[current_column] + to_string(i + 1));
			current_coposition = columns[current_column] + to_string(i + 1);
			current_solution.push_back(current_coposition);
			string current_string = find_postions(chessBoard, current_column + 1);
			if (current_string != "fail") { //if it wasn't a fail, you can return the value since solution is found
				return current_string + columns[current_column] + to_string(i + 1);
			}
			else { //it was a failed move, reverse changes.
				current_solution.pop_back();
				chessBoard = temp_chessBoard;
			}
		}
	}
	return "fail"; //checked all 8 positions, none of them are valid
}

string find_solution(map<char, vector<int>>chessBoard, int current_column) { //recursive function, finds the first solution for the problem
	/*
		the function takes in the chessboard and the current column it is supposed to look on for spaces as parameters
		when it tries to place a queen it access the global variables and when it finds the solution
	*/
	//display_chessboard(chessBoard);
	//base case, if the column is 'h' (last column) and there are possible places to place the queen(there's a 0 on the map) then the solution is found
	if (find(chessBoard[columns[current_column]].begin(), chessBoard[columns[current_column]].end(), 0) != chessBoard[columns[current_column]].end()) { // check if there's a 0 in this column
		if (columns[current_column] == 'h') { // if its the last column
			auto it = find(chessBoard[columns[current_column]].begin(), chessBoard[columns[current_column]].end(), 0); //finding the index of the column
			int index = distance(chessBoard[columns[current_column]].begin(), it);
			return columns[current_column] + to_string(index + 1) + " ";
		}
	}
	else {
		return "fail";
	}
	map<char, vector<int>>temp_chessBoard = chessBoard; //create a coppy of the chessboard, instead of removing last queen, I overwrite the board with how it was before placing her
	for (int i = 0; i < 8; i++) { //check all 8 positions
		if (chessBoard[columns[current_column]][i] != 1) { //if are free try to place a queen
			add_queen(chessBoard, columns[current_column] + to_string(i + 1));
			string current_string = find_solution(chessBoard, current_column + 1);
			if (current_string != "fail") { //if it wasn't a fail, you can return the value since solution is found
				return current_string + columns[current_column] + to_string(i + 1) + " ";
			}
			else { //it was a failed move, overwrite the board
				chessBoard = temp_chessBoard;
			}
		}
	}
	return "fail"; //checked all 8 positions, none of them are valid
}

void eight_queens() {
	//set up for the board
	map<char, vector<int>>chessBoard; //map that links a character to a vector of intigers, chessboard['a'][0] would be the first entry, a1
	vector<string>solution_list;

	vector<int> empty_row = { 0,0,0,0,0,0,0,0 }; //initialize the chessboard with all entries being valid
	chessBoard['a'] = empty_row;
	chessBoard['b'] = empty_row;
	chessBoard['c'] = empty_row;
	chessBoard['d'] = empty_row;
	chessBoard['e'] = empty_row;
	chessBoard['f'] = empty_row;
	chessBoard['g'] = empty_row;
	chessBoard['h'] = empty_row;

	string first_solution = find_solution(chessBoard, 0); //finds single solution and returns it as a string
	cout << "first solution it finds: " << endl << first_solution << endl << endl;

	string a_solution = find_postions(chessBoard, 0); //find positions-solutions starting from columns[0] - a
	//display_chessboard(chessBoard);
}

int main()
{
	eight_queens();
	cout << "all possible combination go as follows:" << endl;
	for (int i = 0; i < solutions.size(); i++) {
		for (int j = 0; j < solutions[i].size(); j++) {
			cout << solutions[i][j] + " ";
		}
		cout << endl;
	}
}