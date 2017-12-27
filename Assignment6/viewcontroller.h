//FileName:		viewcontroller.h
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		This file defines the header for the Viewcontroller class
//This class is responsible for setting up SDL and handing user input.

#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <SDL_mixer.h>
#include <SDL.h>  //Include SDL.h before glm.hpp
#include "worldModel.h"
#include <glm.hpp>
using namespace glm;

class Viewcontroller
{
	private:
		bool quit;

		//Variables to control user movement
		vec3 eye;
		vec3 aim;
		vec3 up;
		vec3 temp;
		vec3 bossPos;
		mat4 view_matrix;
		double MOVEANGLE;
		double LOOKANGLE;
		double moveForward;
		double moveSideways;
		double baseX;
		double baseY;

		WorldModel theWorld;  //Model we want to render

		SDL_Window *window;
		SDL_GLContext ogl4context;

		//variables to keep track of the information
		bool start;
		bool cube1Collected;
		bool cube2Collected;
		bool cube3Collected;
		bool cube4Collected;
		bool begin;
		bool end;
		bool win;

		//Sound effects
		Mix_Chunk *footsteps;
		Mix_Chunk *background;
		Mix_Chunk *collected;
		Mix_Chunk *gunshot;
		Mix_Chunk *applause;
		Mix_Chunk *boo;
	public:
		Viewcontroller();

		bool init();  //initializes SDL, GLEW, and OpenGL
		void display();
		void run();
		bool handleEvents(SDL_Event *theEvent);
		void updateLookAt();
};

#endif