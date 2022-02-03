#include "sphere.h"
#include <iostream>
#include <algorithm>
#include <fstream>      // std::ofstream
#include <math.h>
#include <vector>
#include <SDL.h>
#undef main
#include <glm/glm.hpp>
#include <thread>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

using namespace std;
using namespace glm;

const int WIDTH = 800;
const int HEIGHT = 600;

Clock::time_point timeNow; //time of current frame
Clock::time_point oldTime; //time of previous frame

SDL_Event event;

#include "Model.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h" 
//#include "stdafx.h"

using namespace std;
using namespace glm;

vector<Shape*> Shapes;

Shape* model = new Model("mustang.obj", vec3(0.0f, 0.0f, -20.0f));
Shape* redsphere = new Sphere(4.0f, vec3(0.0f, 0.0f, -22.5f), vec3(1.0f, 0.32f, 0.36f), 0.85f, 0.8f);
Shape* yellowsphere = new Sphere(2.0f, vec3(5.0f, 3.0f, -12.5f), vec3(0.90f, 0.76f, 0.46f), 0.8f, 0.75f);
Shape* bluesphere = new Sphere(3.0f, vec3(5.0f, 0.0f, -25.0f), vec3(0.65f, 0.77f, 0.97f), 0.75f, 0.65f);
Shape* greyphere = new Sphere(4.0f, vec3(-5.5f, 0.0f, -17.5f), vec3(0.90f, 0.90f, 0.90f), 0.7f, 0.6f);
Shape* plainplane = new Plane(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, -5.0f,0.0f), vec3(236.0f/256.0f, 193.0f / 256.0f, 178.0f / 256.0f), 0.3f, 0.3f); // y is upwards // creme dusk	
Shape* exodia = new Triangle(vec3(5.0f, 3.0f, -10.0f), vec3(6.9f, 3.0f, -10.0f), vec3(3.4f, 5.5f, -10.0f), vec3(0.0, 0.6, 1.0), vec3(-0.4, -0.4, 1.0), vec3(0.4, -0.4, 1.0), vec3(0.0f), vec3(0.6f,0.6f,0.0f), 0.5f, 0.7f);

float ambient_light = 0.3f;
vec3 light_intensity = vec3(1.0f, 1.0f, 1.0f);

vec3 Origin = vec3(0.0f); //cameras origin

vec3 area_light_position = vec3(0.0f,20.0f,0.0f);
vec3 area_light_dimensions = vec3(12.0f, 0.0f, 12.0f);

#define PI 3.14159265

vec3** image = new vec3 * [WIDTH];
SDL_Window* SDL_window = NULL;
SDL_Surface* screenSurface = NULL;
bool option_shadows = true;
bool option_reflections = true;
const float samples = 10;
const int max_depth = 3;
const int threadCount = 32;

bool initSDL(SDL_Window*& window, SDL_Surface*& screenSurface)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}
	else
	{
		//create the window
		window = SDL_CreateWindow("SDL Version", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			cout << "SDL Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			return false;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
			return true;
		}
	}
};

void closeSDL(SDL_Window*& window)
{
	SDL_DestroyWindow(window);
	SDL_Quit();
};

Uint32 convertColour(vec3 colour)
{
	int tt;
	Uint8 r, g, b;

	tt = (int)(colour.r * 255);
	if (tt <= 255) r = tt; else r = 255;
	tt = (int)(colour.g * 255);
	if (tt <= 255) g = tt; else g = 255;
	tt = (int)(colour.b * 255);
	if (tt <= 255) b = tt; else b = 255;

	Uint32 rgb;

	//check which order SDL is storing bytes
	rgb = (r << 16) + (g << 8) + b;

	return rgb;
};

void PutPixel32_nolock(SDL_Surface*& surface, int x, int y, Uint32 colour)
{
	Uint8* pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = colour;
};

vec3 reflection(vec3 p,vec3 new_p, vec3 v,vec3 n, int Depth) {
	if (Depth < 1) {
		return vec3(0);
	}

	bool reflection_intersection = false;
	float temp;

	vec3 n_unit = normalize(n);
	new_p = new_p + n * FLT_EPSILON;
	v = v - 2.0f * n_unit * dot(normalize(v), n_unit);

	for (int i = 0; i < Shapes.size(); i++) {
		reflection_intersection = Shapes[i]->intersection(new_p, v, temp);
		if (reflection_intersection){

			vec3 temp_p = new_p + v * temp;
			new_p = v * temp;

			vec3 light_direction = glm::normalize(area_light_position - new_p);
			if (Depth == 1) {
				return Shapes[i]->Compute_colour(ambient_light, light_direction, light_intensity, new_p, v, p);
			}
			else {
				vec3 finalCol = Shapes[i]->Compute_colour(ambient_light, light_direction, light_intensity, new_p, v, p);
				n = normalize(temp_p - Shapes[i]->getCentre());
				return finalCol + Shapes[i]->metallic * reflection(temp_p, new_p, v, n, Depth - 1);
			}
		}
	}
	return vec3(0);
}

void MultiThread(int start, int end) {
	vec3 finalCol;

	float PixelNdx, PixelNdy, PixelRdx, PixelRdy, Iar, tanvalue, PCameraX, PCameraY;

	Iar = WIDTH / (float)HEIGHT;
	tanvalue = tanf(30 * PI / 180.0);
	bool Intersection;
	float t, temp, min_t, ColorVal;
	int i, whichone;
	vec3 ttvec, dir, IntPt;
	vec3 point_of_intersection, light_direction;
	vector<float> t_arr;
	vector<vec3> color_arr;

	vec3 light_step = area_light_dimensions / samples;

	//loop through pixels
	for (int x = start; x < end; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			t_arr.clear();
			color_arr.clear();

			PixelNdx = (x + 0.5) / (float)WIDTH;
			PixelNdy = (y + 0.5) / (float)HEIGHT;
			PixelRdx = 2 * PixelNdx - 1.0;
			PixelRdy = 1.0 - 2 * PixelNdy;
			PixelRdx = PixelRdx * Iar;

			PCameraX = PixelRdx * tanvalue;
			PCameraY = PixelRdy * tanvalue;

			ttvec.x = PCameraX;
			ttvec.y = PCameraY;
			ttvec.z = -1.0;
			//dir need to be normalized
			dir = normalize(ttvec);

			int k = 1;

			bool Intersection, light_intersection;
			light_intersection = false;
			for (int i = 0; i < Shapes.size(); i++) { //for every object
				Intersection = Shapes[i]->intersection(Origin, dir, t);
				if (Intersection){ // see if ray gits something
					bool shadow = false;
					t_arr.push_back(t);
					point_of_intersection = dir * t;
					finalCol = vec3(0);
					if (option_shadows) { // if shadows are enables
						for (float k = 1; k < samples + 1; k++) { // sample light
							light_direction = glm::normalize((area_light_position + light_step * k) - point_of_intersection);
							for (int j = 0; j < Shapes.size(); j++) { // for every other  object
								if (i == j) {
									continue;
								}
								light_intersection = Shapes[j]->intersection(point_of_intersection, light_direction, temp);
								if (light_intersection) {
									if (!shadow) {
										finalCol = Shapes[i]->Compute_colour(ambient_light, light_direction, light_intensity, point_of_intersection, dir, Origin) * ambient_light * 3.0f;
										shadow = true;
									}
									else {
										finalCol -= ambient_light / (samples * 2.0f);
									}
									break;
								}
							}
							if (!shadow) {
								finalCol = Shapes[i]->Compute_colour(ambient_light, light_direction, light_intensity, point_of_intersection, dir, Origin);
							}
						}
						for (int i = 0; i < 3; i++) {
							if (finalCol[i] < 0) {
								finalCol[i] = 0;
							}
						}
					}
					else {
						light_direction = glm::normalize(area_light_position - point_of_intersection);
						finalCol = Shapes[i]->Compute_colour(ambient_light, light_direction, light_intensity, point_of_intersection, dir, Origin);
					}
					if (option_reflections) {
						vec3 n = point_of_intersection - Shapes[i]->getCentre();
						//vec3 n_unit = normalize(n);
						//vec3 new_p = point_of_intersection + n * FLT_EPSILON;
						//vec3 v = dir - 2.0f * n_unit * dot(normalize(dir), n_unit);
						finalCol += Shapes[i]->metallic * reflection(Origin, point_of_intersection, dir, n, max_depth);
					}
					color_arr.push_back(finalCol);
				}
			}

			if (t_arr.size() == 0)
			{
				image[x][y] = vec3(1.0);
				PutPixel32_nolock(screenSurface, x, y, convertColour(image[x][y]));
			}
			else {
				float min_t = 1000.0;
				int whichone = 0;
				for (int i = 0; i < t_arr.size(); i++) {
					if (t_arr[i] < min_t) {
						whichone = i; min_t = t_arr[i];
					}
				}

				image[x][y] = color_arr[whichone];

				PutPixel32_nolock(screenSurface, x, y, convertColour(image[x][y]));
			}
		}
		SDL_UpdateWindowSurface(SDL_window);
	}
}

int main()
{
	Shapes.push_back(redsphere);
	Shapes.push_back(yellowsphere);
	Shapes.push_back(bluesphere);
	Shapes.push_back(greyphere);
	Shapes.push_back(plainplane);
	Shapes.push_back(exodia);

	std::cout << "to turn the shadows on or off please press S" << std::endl;
	std::cout << "to turn the reflections on or off please press R" << std::endl;

	for (int i = 0; i < WIDTH; i++) image[i] = new vec3[HEIGHT];

	///SECTION - SDL Setup
	if (!initSDL(SDL_window, screenSurface)) return -1;

	bool quit = false;
	bool waiting_for_input;
	int frameTime;
	while (!quit) {

		timeNow = Clock::now();

		waiting_for_input = true;
		//Launch a thread
		std::vector<std::thread> threads;
		for (int i = 0; i < threadCount; i++) {
			threads.push_back(std::thread(MultiThread, (i * WIDTH) / threadCount, (i + 1) * WIDTH / threadCount));
		}

		for (int i = 0; i < threadCount; i++) {
			threads[i].join();
		}

		oldTime = timeNow;
		timeNow = Clock::now();
		// Time taken for this frame in nanoseconds
		frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - oldTime).count();
		std::cout << "Time taken for this frame in milliseconds " << frameTime << std::endl;
		std::cout << "FPS: " << 1 / (frameTime * 0.001) << std::endl;


		const Uint8* state = SDL_GetKeyboardState(NULL);

		//wait for input and then restart raycasting
		while (waiting_for_input && !quit) {

			while (SDL_PollEvent(&event) != 0) {
				state = SDL_GetKeyboardState(NULL);
				if (state[SDL_SCANCODE_ESCAPE]) {
					quit = true;
				}
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					if (state[SDL_SCANCODE_S]) {
						option_shadows = !option_shadows;
						waiting_for_input = false;
						std::cout << "changing shadows value " << option_shadows << std::endl;
					}
					if (state[SDL_SCANCODE_R]) {
						option_reflections = !option_reflections;
						waiting_for_input = false;
						std::cout << "changing reflections value " << option_reflections << std::endl;
					}
				}
			}
		}
	}

	//// Save result to a PPM image
	//ofstream ofs("./test.ppm", std::ios::out | std::ios::binary);
	//ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
	//for (int y = 0; y < HEIGHT; ++y)
	//{
	//	for (int x = 0; x < WIDTH; ++x)
	//	{
	//		ofs << (unsigned char)(std::min((float)1, (float)image[x][y].x) * 255) <<
	//			(unsigned char)(std::min((float)1, (float)image[x][y].y) * 255) <<
	//			(unsigned char)(std::min((float)1, (float)image[x][y].z) * 255);
	//	}
	//}
	//ofs.close();

	closeSDL(SDL_window);
	return 0;
}