#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

struct most_common_words { //this is basically a struct for 2D array of strings and intigers - used to store 66 mosc common words //https://www.quora.com/How-do-I-store-different-data-types-in-a-single-array-in-C
	vector<string> words_list;
	vector<int> words_count;
};

class Node { //a class for a node, contains a word, and two pointers, for left and right child, count, and depth
public:
	string word;
	int count; //using a varaible count is much more space and later time efficient than repeating a word
	int depth; //depth is used by the program to know whether it requires rotatations
	Node* left;
	Node* right;
};

class BinarySearchTree {

public:
	Node* root_node = nullptr; //here a roon node is initialised as a null pointer
	most_common_words mylist; //needed for task 2.2 a list of most common words
	void add_node(string word) { //when the program tries to add a node it firstly needs to check that a root node exist, and it has a value
		if (root_node == nullptr) { //if the root is empty i.e. contains a null pointer, instead of adding a word, make the root contain that value
			root_node = new Node();
			root_node->word = word;
			root_node->count = 1;
			root_node->depth = 1;
			root_node->left = nullptr;
			root_node->right = nullptr;
		}
		else if (root_node->word == word) { //if the word is the root node
			root_node->count++;
		}
		else { //if we have a root node, try adding new node if root is empty, we would be compating a value to null, which would throw an error
			this->_add_node(word, root_node, root_node, "root");
		}
	}

	Node* rotate_right(Node* current_node, int change) { // rotate right, current node becomes right parent of it's left child and it's right children becomes current's left children, 
		Node* root = current_node->left;
		current_node->left = root->right;
		root->right = current_node;
		current_node->depth -= change;
		return root;
	}

	Node* rotate_left(Node* current_node, int change) { // rotate left, current node becomes left parent of it's right child and  and it's left children becomes current's right children, 
		Node* root = current_node->right;
		current_node->right = root->left;
		root->left = current_node;
		current_node->depth -= change;
		return root;
	}

	bool rotate(bool changed, Node* current_node, Node* parent_node, string direction, string left_right, int change) {

		/*
		This function checks whether the tree needs a single or double rotation and calculates it's depth
		left right is a variable that tells this function whether it has to calculate a single or double rotation in left or right direction,
		whereas direction says whether it's parent's right or left child
		this function is used by both add and remove functions, therefore there's a change int which is either 1 or minus 1,
		*/

		if (left_right == "left") {
			if (changed && current_node->right == nullptr) { // if we should increase the depth, and right is an empty pointer then its a single rotation
				current_node->depth += change;
				if (current_node->depth == 3) { // we know that right is an empty pointer so if the depth of this, is 3 then we know it should rotate
					if (current_node->left->right == nullptr) {
						if (direction == "root") { // I need to use parents (I am too stupid to know why it doesn't work otherwise)
							root_node = rotate_right(current_node, 2);
						}
						else if (direction == "left") {
							parent_node->left = rotate_right(current_node, 2);
						}
						else {
							parent_node->right = rotate_right(current_node, 2);
						}
					}
					else {
						current_node->left = rotate_left(current_node->left, 1);
						if (direction == "root") {
							root_node = rotate_right(current_node, 2);
						}
						else if (direction == "left") {
							parent_node->left = rotate_right(current_node, 2);
						}
						else {
							parent_node->right = rotate_right(current_node, 2);
						}
					}
				}
				return true; // we increase this depth, therefore one above might increase aswell, return true
			}
			else if (changed && current_node->left->depth > current_node->right->depth) { // possible double rotation, 
				current_node->depth += change;
				if (current_node->depth - current_node->left->depth > 2) { //right heavy
					if (current_node->left->right == nullptr) { // if true, left subtree right heavy
						if (direction == "root") {
							root_node = rotate_right(current_node, 2);
						}
						else if (direction == "left") {
							parent_node->left = rotate_right(current_node, 2);
						}
						else {
							parent_node->right = rotate_right(current_node, 2);
						}
					}
					else if (current_node->left->left == nullptr) { // if true, left subtree left heavy
						current_node->left = rotate_left(current_node->left, 1);
						if (direction == "root") {
							root_node = rotate_right(current_node, 2);
						}
						else if (direction == "left") {
							parent_node->left = rotate_right(current_node, 2);
						}
						else {
							parent_node->right = rotate_right(current_node, 2);
						}
					}
					else if (current_node->left->left->depth > current_node->left->right->depth) { // if true, left subtree right heavy
						if (direction == "root") {
							root_node = rotate_right(current_node, 2);
						}
						else if (direction == "left") {
							parent_node->left = rotate_right(current_node, 2);
						}
						else {
							parent_node->right = rotate_right(current_node, 2);
						}
					}
					else { // if true, left subtree left heavy
						current_node->left = rotate_left(current_node->left, 1);
						if (direction == "root") {
							root_node = rotate_right(current_node, 2);
						}
						else if (direction == "left") {
							parent_node->left = rotate_right(current_node, 2);
						}
						else {
							parent_node->right = rotate_right(current_node, 2);
						}
					}
				}
				return true; // we increase this depth, therefore one above might increase aswell, return true
			}
			return false; //the difference is 0, or 1, thereforee the tree is not offset, does not need rebalancing
		}
		else {
			if (changed && current_node->left == nullptr) {
				current_node->depth += change;
				if (current_node->depth == 3) {
					if (current_node->right->left == nullptr) {
						if (direction == "root") {
							root_node = rotate_left(current_node, 2);
						}
						if (direction == "left") {
							parent_node->left = rotate_left(current_node, 2);
						}
						else {
							parent_node->right = rotate_left(current_node, 2);
						}
					}
					else {
						current_node->right = rotate_right(current_node->right, 1);
						if (direction == "root") {
							root_node = rotate_left(current_node, 2);
						}
						if (direction == "left") {
							parent_node->left = rotate_left(current_node, 2);
						}
						else {
							parent_node->right = rotate_left(current_node, 2);
						}
					}
				}
				return true;
			}
			else if (changed && current_node->right->depth > current_node->left->depth) {
				current_node->depth += change;
				if (current_node->depth - current_node->right->depth > 2) { //right heavy
					if (current_node->right->left == nullptr) { // if true, right subtree right heavy
						if (direction == "root") {
							root_node = rotate_left(current_node, 2);
						}
						if (direction == "left") {
							parent_node->left = rotate_left(current_node, 2);
						}
						else {
							parent_node->right = rotate_left(current_node, 2);
						}
					}
					else if (current_node->right->right == nullptr) { // if true, right subtree left heavy
						current_node->right = rotate_right(current_node->right, 1);
						if (direction == "root") {
							root_node = rotate_left(current_node, 2);
						}
						if (direction == "left") {
							parent_node->left = rotate_left(current_node, 2);
						}
						else {
							parent_node->right = rotate_left(current_node, 2);
						}
					}
					else if (current_node->right->right->depth > current_node->right->left->depth) { // if true, right subtree right heavy
						if (direction == "root") {
							root_node = rotate_left(current_node, 2);
						}
						if (direction == "left") {
							parent_node->left = rotate_left(current_node, 2);
						}
						else {
							parent_node->right = rotate_left(current_node, 2);
						}
					}
					else { // if true, right subtree left heavy
						current_node->right = rotate_right(current_node->right, 1);
						if (direction == "root") {
							root_node = rotate_left(current_node, 2);
						}
						if (direction == "left") {
							parent_node->left = rotate_left(current_node, 2);
						}
						else {
							parent_node->right = rotate_left(current_node, 2);
						}
					}
				}
				return true;
			}
			return false;
		}
	}

	bool _add_node(string word, Node* current_node, Node* parent_node, string direction) {

		/*
		_add_node method creates new nodes if the word is not present and adds 1 to the counter of the node if a word with that node is present
		when it adds a word, when returning it adds 1 to the depth of the tree, since the bool value, if it adds a new node it increases by 1 the depth of the node
		where it should, in this method, in it's backwards call it checks for rotations.
		*/

		if (word < current_node->word) { // if the word we want to add is smaller than the word of the current node, we call recursively left,
			if (current_node->left != nullptr) { // if there's no null pointer, we need to go deeper the treem
				bool added = _add_node(word, current_node->left, current_node, "left"); //call this function reccursively, if we added a node and this node should increase in depth, this will be true
				return rotate(added, current_node, parent_node, direction, "left", 1); // this function handles roatation, single, double, right or left
			}
			else { //if the pointer is empty, we have a place where we can put it
				current_node->left = new Node();
				current_node->left->word = word;
				current_node->left->count = 1;
				current_node->left->depth = 1;
				current_node->left->left = nullptr;
				current_node->left->right = nullptr;
				if (current_node->right == nullptr) {
					current_node->depth++;
				}
				return true;
			}
		}
		else if (word == current_node->word) { // if it's equal to current word, it's repeated increase it's count.
			current_node->count++;
			return false;
		}
		else { // if the word we want to add is smaller than the word of the current node, we call recursively right,
			if (current_node->right != nullptr) {
				bool added = _add_node(word, current_node->right, current_node, "right");
				return rotate(added, current_node, parent_node, direction, "right", 1);
			}
			else {
				current_node->right = new Node();
				current_node->right->word = word;
				current_node->right->count = 1;
				current_node->right->depth = 1;
				current_node->right->left = nullptr;
				current_node->right->right = nullptr;
				if (current_node->left == nullptr) {
					current_node->depth++;
				}
				return true; // we added a new node therefore return true
			}
		}
	}

	bool delete_node(Node* current_node, Node* parent_node, string left_or_right, string word_to_delete) {
		if (current_node == nullptr) {// we reached a dead end, word is not present
			cout << "it's not present - wasn't deleted" << endl;
			return false;
		}
		else if (word_to_delete == current_node->word) { // we found the node with the word, time to delete it
			cout << "found, deleting" << endl;
			if (current_node->left == nullptr && current_node->right == nullptr) { // case 1, the node has no children, make the parent pointer empty
				//cout << "case1, parent is " << parent_node->word << endl; //used for debugging
				if (left_or_right == "left") {
					parent_node->left = nullptr;
				}
				else {
					parent_node->right = nullptr;
				}
				return true;
			}
			else if (current_node->left == nullptr || current_node->right == nullptr) { // case 2, has only 1 child, change the parent pointer to point at the child
				// cout << "case2" << endl; //used for debugging
				if (current_node->left == nullptr) {
					if (left_or_right == "left") {
						parent_node->left = current_node->right;
					}
					else {
						parent_node->right = current_node->right;
					}
				}
				else {
					if (left_or_right == "left") {
						parent_node->left = current_node->left;
					}
					else {
						parent_node->right = current_node->left;
					}
				}
				return true;
			}
			else { // case 3, has two children, find the right smallest child
				//cout << "case3" << endl; //used for debugging
				Node* smallest = current_node->right;
				Node* smallest_parent = current_node;
				string side = "right";
				while (smallest->left != nullptr) {
					side = "left";
					smallest_parent = smallest;
					smallest = smallest->left;
				}
				if (smallest->right != nullptr) {
					if (side == "right") {
						current_node->right = smallest->right;
					}
					else {
						smallest_parent->left = smallest->right;
					}
				}
				else {
					if (side == "right") {
						current_node->right = nullptr;
					}
					else {
						smallest_parent->left = nullptr;
					}
				}
				current_node->word = smallest->word;
				current_node->count = smallest->count;
				return true;
			}
		}
		else if (word_to_delete < current_node->word) { //if the the word we are looking for is smaller than the current node choose left node from the parent
			bool deleted = delete_node(current_node->left, current_node, "left", word_to_delete);
			return rotate(deleted, current_node, parent_node, left_or_right, "right", -1);
		}
		else { //if it is not smaller than the parent or equal, it has to be bigger, choose right pointer
			bool deleted = delete_node(current_node->right, current_node, "right", word_to_delete);
			return rotate(deleted, current_node, parent_node, left_or_right, "left", -1);
		}
	}

	int search(Node* parent, string target) {
		if (parent == nullptr) {//if the node has a null pointer, we reatched a node that doesn't exist, a word is not present
			cout << "it's not present" << endl;
			return 0;
		}
		else if (target == parent->word) { //if the pointer is equal to the word we are looking for, we found it we can display its count
			return parent->count;
		}
		else if (target < parent->word) { //if the the word we are looking for is smaller than the current node choose left node from the parent
			return search(parent->left, target);
		}
		else { //if it is not smaller than the parent or equal, it has to be bigger, choose right pointer
			return search(parent->right, target);
		}
	}

	void output_right_left(Node* parent_node) {
		//it's very similar to inorder traverse, however it adds each element to the list of words
		//uses insertion sort and removes the 67th element
		if (parent_node == nullptr) {//this is recursive therefore the base case is if the working pointer is empty
			return;
		}
		output_right_left(parent_node->left); //go to the most left one, smallest one first,
		if (mylist.words_list.size() == 0 && parent_node->word != "the" && parent_node->word != "a") { // if the list is empty add the word, with the exception of and & the 
			mylist.words_list.push_back(parent_node->word);
			mylist.words_count.push_back(parent_node->count);
		}
		else { // if the list is not empty, and the word is on the whitelist, use insertion sort to add it.
			if (parent_node->word != "the" && parent_node->word != "a") {
				for (int i = 0; i < mylist.words_list.size(); i++) {
					if (parent_node->count <= mylist.words_count[mylist.words_list.size() - 1]) {
						mylist.words_list.push_back(parent_node->word);
						mylist.words_count.push_back(parent_node->count);
						break;
					}
					else if (parent_node->count >= mylist.words_count[i]) { //https://www.geeksforgeeks.org/vector-insert-function-in-c-stl/
						mylist.words_list.insert(mylist.words_list.begin() + i, 1, parent_node->word);
						mylist.words_count.insert(mylist.words_count.begin() + i, 1, parent_node->count);
						break;
					}
				}
				if (mylist.words_list.size() > 66) { // remove last element
					mylist.words_list.pop_back();
					mylist.words_count.pop_back();
				}
			}
		}
		output_right_left(parent_node->right); // go to the right node
	}

	void traverse_tree(Node* parent_node) { // used for debugging, inorder tree traversal
		if (parent_node == nullptr) {
			return;
		}
		traverse_tree(parent_node->left);
		cout << parent_node->word << " " << parent_node->count << " " << parent_node->depth << " " << endl;
		traverse_tree(parent_node->right);
	}

};

struct edge { //edges, do not link to other vertecies, becasue the graph uses a list of them, hoever stores their index in that list
	string nodes;
	int index; //instead of using pointers I decided it would be easier for me to store indexes of the vertices list
	float weight;
};


class Vertex { //the vertex consists of a word, lists of sonnets they appear in, any edges, and their weights, adjacency list form.
public:
	string word;
	vector<int> sonnets;
	vector<edge> edges;
	float tentative_weight;
	vector<string> path;
};

class Graph {
public:
	vector<Vertex> vertices; //list of all the edges in the graph

	void populate(BinarySearchTree shakespeares_tree) { // copy the result of the 66 most common words into vertices
		Vertex temp_vertex;
		for (int i = 0; i < 66; i++) {
			temp_vertex = Vertex();
			temp_vertex.word = shakespeares_tree.mylist.words_list[i];
			temp_vertex.tentative_weight = FLT_MAX; //might aswell do it here instead of dijkstra //http://www.cplusplus.com/reference/limits/numeric_limits/
			this->vertices.push_back(temp_vertex);
		}
	}

	void calculate_weights() {
		/*
		the function calculates the weights of the edges between the words
		for vertex compare it to every other vertex's sonnet list
		to optimise it, and since its undirected, once it calculates weight from A to B it uses the same weight for edge B to A
		*/
		int current_weight, a, b, start = 1;
		float final_weight;
		edge temp_edge;
		for (int i = 0; i < vertices.size(); i++) {
			for (int j = start; j < vertices.size(); j++) {
				current_weight = 0;
				a = 0;
				b = 0;
				while (a != vertices[i].sonnets.size() - 1 && b != vertices[j].sonnets.size() - 1) {
					/*
					he basically two arrays are compared to each other, if elements are equal, the weight is increased, once the comparison is done,
					one of the arrays index is incremented based on the one which represents a smaller value
					if a reaches the end, make the b reach its end too. when both reach last element loop terminates
					*/
					if (vertices[i].sonnets[a] == vertices[j].sonnets[b]) {
						current_weight++;
					}
					if (a != vertices[i].sonnets.size() - 1 && vertices[i].sonnets[a] < vertices[j].sonnets[b]) {
						a++;
					}
					else {
						b++;
					}
				}
				if (current_weight != 0) { // check if there is some link so we dont divide by 0
					final_weight = 1.0 / current_weight; // make it inversely proportional 
					temp_edge = edge();
					temp_edge.index = j;
					temp_edge.nodes = vertices[j].word;
					temp_edge.weight = final_weight;
					vertices[i].edges.push_back(temp_edge);
					temp_edge = edge();
					temp_edge.index = i;
					temp_edge.nodes = vertices[i].word;
					temp_edge.weight = final_weight;
					vertices[j].edges.push_back(temp_edge);
				}
			}
			start++; //this makes sure that miss half of the comparisons, A made comparisons with everything, we shouldnt compare A again
		}
	}

	void graph_output() { //used for debugging, prints the vertex, and it's edges with their weights
		for (int i = 0; i < 66; i++) {
			cout << "vertex " << vertices[i].word << endl;
			for (int j = 0; j < vertices[i].edges.size(); j++) {
				cout << vertices[i].edges[j].nodes << " " << vertices[i].edges[j].weight << endl;
			}
			cout << endl;
		}
	}

	bool is_strongly_connected() { //simple algorithm, if every vertex has total - 1 edges (-1 due to not pointing to itself) then every vertex is connected with every vertex, Strongly connected.
		for (int i = 0; i < vertices.size(); i++) { // for all the vertices
			if (vertices[i].edges.size() != vertices.size() - 1) { // if they have 65 edges, they are strongly connected with teach vertex
				return false; //if they dont have 65 edges, it's not strongly connected
			}
		}
		return true; //if it looped through all edges, all of them have 65 edges. therefore strongly connected
	}

	bool is_connected() { //breath first search, it will fill the memory a bit will all the vectors, since it adds the visited too
		vector<Vertex> nodes;
		vector<Vertex> visited_nodes;
		bool in_visited;
		Vertex current_node;
		nodes.push_back(vertices[0]);
		while (!nodes.empty()) {
			in_visited = false;
			current_node = nodes[0];
			nodes.erase(nodes.begin()); //http://www.cplusplus.com/reference/vector/vector/erase/
			for (int i = 0; i < visited_nodes.size(); i++) {
				if (current_node.word == visited_nodes[i].word) {
					in_visited = true;
					break;
				}
			}
			if (!in_visited) {
				visited_nodes.push_back(current_node);
				if (visited_nodes.size() == 66) {
					break;
				}
				for (int i = 0; i < current_node.edges.size(); i++) {
					nodes.push_back(vertices[current_node.edges[i].index]);
				}
			}
		}
		return (visited_nodes.size() == vertices.size());
	}

	vector<string> Dijkstra(int start_word_index, int end_word_index) {

		/*
		this is a plain simple off book implementation of dijkstra, only exception is that I do not iterate over vertices, but I iterate over it's edges and use their indexes
		this way I do not store a pointer to the vertex but a key to it in the vertex vector.
		this makes it this code more complicated but works exactly the same.
		*/

		Vertex working_vertex = vertices[start_word_index];
		Vertex source_vertex = vertices[start_word_index];
		Vertex target_vertex = vertices[end_word_index];
		working_vertex.tentative_weight = 0;
		vector<Vertex> visited_vertices;
		float min;
		while (working_vertex.word != target_vertex.word) {
			//cout << "working vertex: " << working_vertex.word << endl; // used for debugging, shows currently active node
			for (int i = 0; i < working_vertex.edges.size(); i++) {
				if (working_vertex.tentative_weight + working_vertex.edges[i].weight < vertices[working_vertex.edges[i].index].tentative_weight) {
					vertices[working_vertex.edges[i].index].tentative_weight = working_vertex.tentative_weight + working_vertex.edges[i].weight;
					vertices[working_vertex.edges[i].index].path.push_back(working_vertex.word);
				}
			}
			visited_vertices.push_back(working_vertex);
			min = FLT_MAX; //http://www.cplusplus.com/reference/limits/numeric_limits/
			for (int i = 0; i < vertices.size(); i++) {
				for (int j = 0; j < visited_vertices.size(); j++) {
					if (vertices[i].word == visited_vertices[j].word) {
						break;
					}
					if (j == visited_vertices.size() - 1 && vertices[i].tentative_weight < min) {
						working_vertex = vertices[i];
						min = vertices[i].tentative_weight;
					}
				}
			}
		}
		working_vertex.path.push_back(target_vertex.word); // when it reaches end, the loop stops, therefore add last element to the path
		return working_vertex.path;
	}

	void most_pop_sonnet(vector<string> path) {

		/*
		this function uses path and finds the words to find indexes of the words using a binary search, so it can access the vertecies that store the sonnets nummbers
		later on it adds all of the sonnets to one array, uses bubble sort to sort it, and counts through them finding the most popular sonnet
		*/

		int min, max, midpoint;
		bool not_equal;
		vector<int> indexes, all_sonnets;
		for (int i = 0; i < path.size(); i++) {
			min = 0;
			midpoint = 32;
			max = 65;
			not_equal = true;
			while (not_equal) {
				if (path[i] < vertices[midpoint].word) {
					max = midpoint - 1;
				}
				else if (path[i] > vertices[midpoint].word) {
					min = midpoint + 1;
				}
				else {
					indexes.push_back(midpoint);
					not_equal = false;
				}
				midpoint = floor((min + max) / 2);
			}
		}
		for (int i = 0; i < indexes.size(); i++) {
			for (int j = 0; j < vertices[indexes[i]].sonnets.size(); j++) {
				all_sonnets.push_back(vertices[indexes[i]].sonnets[j]);
			}
		}
		bool swap;
		int temp_sonnet;
		for (int i = all_sonnets.size() - 1; i > 0; i--) {
			swap = false;
			for (int j = 0; j < i; j++) {
				if (all_sonnets[j] < all_sonnets[j + 1]) {
					temp_sonnet = all_sonnets[j];
					all_sonnets[j] = all_sonnets[j + 1];
					all_sonnets[j + 1] = temp_sonnet;
					swap = true;
				}
			}
			if (!swap) {
				break;
			}
		}//bellow is used for debugging, it shows all sonnets in which the words from the path appear
	/*	for (int i = 0; i < all_sonnets.size(); i++) {
			cout << all_sonnets[i]<< endl;
		}*/
		int current_sonnet = all_sonnets[0], current_points = 1;
		int most_pop_sonnet = all_sonnets[0], most_points = 1;
		for (int i = 1; i < all_sonnets.size(); i++) {
			if (current_sonnet == all_sonnets[i]) {
				current_points++;
			}
			else {
				if (current_points > most_points) {
					most_pop_sonnet = current_sonnet;
					most_points = current_points;
				}
				current_sonnet = all_sonnets[i];
				current_points = 1;
			}
		}
		cout << "most common sonnet: " << most_pop_sonnet << " has: " << most_points << " points" << endl;
	}
};

BinarySearchTree task_2(BinarySearchTree shakespeares_tree) {

	//part 1 search for a word

	string user_specified_word;
	cout << "Enter a word you want to look for: ";
	cin >> user_specified_word;
	transform(user_specified_word.begin(), user_specified_word.end(), user_specified_word.begin(), ::tolower); //https://www.tutorialspoint.com/how-to-convert-std-string-to-lower-case-in-cplusplus
	cout << "the word is repeated " << shakespeares_tree.search(shakespeares_tree.root_node, user_specified_word) << " time/s" << endl;

	//part 2 most common words

	string temp_word;
	int temp_count;
	bool swap;

	cout << "most common words" << endl;
	shakespeares_tree.output_right_left(shakespeares_tree.root_node);
	for (int i = 65; i > 1; i--) { //simple bubble sort to sort it out, quick sort would be much efficient, but more complex since im working with two arrays at the same time
		swap = false;
		for (int j = 0; j < i; j++) {
			if (shakespeares_tree.mylist.words_list[j + 1] < shakespeares_tree.mylist.words_list[j]) {
				temp_word = shakespeares_tree.mylist.words_list[j];
				temp_count = shakespeares_tree.mylist.words_count[j];
				shakespeares_tree.mylist.words_list[j] = shakespeares_tree.mylist.words_list[j + 1];
				shakespeares_tree.mylist.words_count[j] = shakespeares_tree.mylist.words_count[j + 1];
				shakespeares_tree.mylist.words_list[j + 1] = temp_word;
				shakespeares_tree.mylist.words_count[j + 1] = temp_count;
				swap = true;
			}
		}
		if (!swap) {
			break;
		}
	}
	for (int i = 0; i < 66; i++) {
		cout << shakespeares_tree.mylist.words_list[i] << " " << shakespeares_tree.mylist.words_count[i] << endl;
	}

	return shakespeares_tree;
}

void task_3(BinarySearchTree shakespeares_tree) {

	//these two lines here and bellow are used for debugging
	//shakespeares_tree.traverse_tree(shakespeares_tree.root_node); 
	//cout << endl;

	string user_specified_word;
	cout << "Enter a word you want to delete: ";
	cin >> user_specified_word;
	transform(user_specified_word.begin(), user_specified_word.end(), user_specified_word.begin(), ::tolower); //https://www.tutorialspoint.com/how-to-convert-std-string-to-lower-case-in-cplusplus
	shakespeares_tree.delete_node(shakespeares_tree.root_node, shakespeares_tree.root_node, "root", user_specified_word);

	//shakespeares_tree.traverse_tree(shakespeares_tree.root_node);
	//cout << endl;

}

Graph task_4_5(BinarySearchTree shakespeares_tree) {

	//beggining is very similar to parsing task 1
	//added comments where's there's a difference in code
	//one of the changes is different varaibles initialisations

	fstream textfile;
	string text_line, word;
	int line_number = 0, min, max, midpoint, sonnet_number = 1;
	bool not_equal = true;
	char current_letter;

	Graph Sonnets_map;

	Sonnets_map.populate(shakespeares_tree);//initialise the graph with all the vertices

	textfile.open("Shakespeare.txt");

	for (line_number; line_number < 252; line_number++) { getline(textfile, text_line); } //https://www.w3schools.com/cpp/cpp_files.asp

	while (line_number < 2868) {
		getline(textfile, text_line); //https://www.w3schools.com/cpp/cpp_files.asp
		for (char& c : text_line) {
			if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c == '\'') || (c > 47 && c < 58)) { //modified it so numbers could be counter as words
				word += c;
			}
			else {
				if (!word.empty()) {
					if (word[0] > 47 && word[0] < 58) { // if first character is a number the "word" is a number
						sonnet_number = stoi(word); //convert into an int //http://www.cplusplus.com/reference/string/stoi/
						word = "";
					}
					else {
						transform(word.begin(), word.end(), word.begin(), ::tolower); //https://www.tutorialspoint.com/how-to-convert-std-string-to-lower-case-in-cplusplus
						min = 0;
						max = 65;
						midpoint = 32;
						not_equal = true;
						while (not_equal) { // simple binary search to check if a word is in the most common words and therefore if the sonnet should be added to one of the vertices
							//cout << "word: " << word << " min: " << min << " max: " << max << " midpoint word: " << shakespeares_tree.mylist.words_list[midpoint] << endl; // debugging
							if (word < shakespeares_tree.mylist.words_list[midpoint]) { //if it is smaller than target, move top "pointer" to midpoint
								max = midpoint - 1;
							}
							else if (word > shakespeares_tree.mylist.words_list[midpoint]) { //if it is bigger than target, move bottom "pointer" to midpoint
								min = midpoint + 1;
							}
							else { // if it neighter the case then it has to be equal to the word. therefore add it to the sonnets array
								if ((Sonnets_map.vertices[midpoint].sonnets.size() == 0)) { //if the sonnet array is empty, append
									Sonnets_map.vertices[midpoint].sonnets.push_back(sonnet_number);
								}
								else if (Sonnets_map.vertices[midpoint].sonnets.back() != sonnet_number) { // if it not empty and last item is not currrent sonnet, append the number
									Sonnets_map.vertices[midpoint].sonnets.push_back(sonnet_number);
								}
								not_equal = false; //we reach the target we should finish
							}
							midpoint = floor((min + max) / 2);
							if (min > max) { //if this happens we met in the middle but the word is not in the list of the most common words,
								not_equal = false;
							}
						}
						word = ""; //reset the word
					}
				}
			}
		}
		line_number++;
	}

	textfile.close();

	Sonnets_map.calculate_weights();

	//Sonnets_map.graph_output(); //debugging

	cout << "the graph is strongly connected: " << Sonnets_map.is_strongly_connected() << endl << "the graph is connected: " << Sonnets_map.is_connected() << endl;

	return Sonnets_map;
}

void task_6(Graph map) {

	//ask user for start and end nodes

	string start_word, end_word, word;
	int start_index, end_index;
	cout << "Enter words from the 66 most common words" << endl;
	cout << "What should the start node be: ";
	cin >> start_word;
	cout << "What should the end node be: ";
	cin >> end_word;
	int min = 0, midpoint = 32, max = 65;
	bool not_equal = true;

	//lets find their indexes

	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			word = start_word;
		}
		else {
			word = end_word;
		}
		min = 0;
		midpoint = 32;
		max = 65;
		not_equal = true;
		while (not_equal) {
			if (word < map.vertices[midpoint].word) {
				max = midpoint - 1;
			}
			else if (word > map.vertices[midpoint].word) {
				min = midpoint + 1;
			}
			else {
				break;
			}
			midpoint = floor((min + max) / 2);
			if (min > max) {
				cout << "the word is not in the list" << endl;
				not_equal = false;
			}
		}
		if (i == 0) {
			start_index = midpoint;
		}
		else {
			end_index = midpoint;
		}
	}

	vector<string> path = map.Dijkstra(start_index, end_index);

	cout << "the path from " << start_word << " to " << end_word << " is:" << endl;
	for (int i = 0; i < path.size(); i++) {
		cout << path[i] << " ";
	}
	cout << endl;

	map.most_pop_sonnet(path);
}

int main() {

	//task 1 - parse words, add them to the tree

	//declaration of varaibles for the word parsing

	fstream textfile; //variable for file
	string text_line; //for each line of the file
	int line_number = 0; //the line in which the current line is
	char current_letter;
	string word;
	BinarySearchTree shakespeares_tree; //initialise the tree

	textfile.open("Shakespeare.txt"); //open

	for (line_number; line_number < 252; line_number++) { getline(textfile, text_line); } //skip the first 252 lines // https://www.w3schools.com/cpp/cpp_files.asp

	while (line_number < 2868) { //this while loop goes trough the entirety of the text needed
		getline(textfile, text_line);
		for (char& c : text_line) { // for every character in line //https://www.w3schools.com/cpp/cpp_files.asp
			if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c == '\'')) { //https://d2vlcm61l7u1fs.cloudfront.net/media%2F8dc%2F8dcf9c2f-9c2c-4eff-89e7-5bd03246ee13%2FphptNgIrb.png
				/*
				the program is working with chars, they can be worked as intigers due to their ascii value
				so if the character is betweem 64 and 91 it is in the uppercase letters, i.e. its a letter,
				if its between, 96 and 123 it's an lower case letter, and the words can include apostrophese " ' "
				it needs to contain a \ as the compiler would think about as syntax otherwise,
				so if the character is in the range of lower or upper case or its a ' it can be added to the word string
				*/
				word += c;
			}
			else { //if it is not a letter, it is either an number or punctuation or space therefore it is a new word. 
				if (!word.empty()) { // if the word is not blank, it contains something, add it to the tree
					transform(word.begin(), word.end(), word.begin(), ::tolower); //convert current word to lowercase //https://www.tutorialspoint.com/how-to-convert-std-string-to-lower-case-in-cplusplus

					//remove the apostrphe from the beggining of the word and its end, if word is " ' " dont add it.

					if (word == "\'") { //if the word is an empty quotemark, it's not a word, return
						word = "";
					}
					else if (word[0] == '\'') { //if it begins with an apostrphe, remove it
						word = word.substr(1, word.size());
						string word_to_add = word; //this has to be cast into a string again, because it treats 'a' as a char instead of a string.
						shakespeares_tree.add_node(word_to_add);
					}
					else if (word[word.size() - 1] == '\'') { //if it ends with an apostrphe, -1 end of size() will point at the end of the last letter.
						word = word.substr(0, word.size() - 1);
						string word_to_add = word;
						shakespeares_tree.add_node(word_to_add);
					}
					else {
						string word_to_add = word;
						shakespeares_tree.add_node(word_to_add);
					}
					word = "";
				}
			}
		}
		line_number++;
	}

	textfile.close(); //close

	//task 2 - search words and search for 66 most used ones, the 66 most common words are used for task 4 therefore it return the list

	shakespeares_tree = task_2(shakespeares_tree);

	//task 3 - delete a word and balance the tree

	task_3(shakespeares_tree);

	//task 4 - graph & task 5 checking if connected, it creates the graph therefore it is returned to be passed for task 6

	Graph G = task_4_5(shakespeares_tree);

	//task 6 - djkstra and most common words

	task_6(G);

	string temp;
	cout << "The program has finished, enter exit to quit ";
	// this is used so that the user can see the result of the path and the program doesn't instantly close when the user doesn't use VS
	cin >> temp;

	return 0;
}