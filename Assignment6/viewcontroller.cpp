//FileName:		Viewcontroller.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		This file defines the methods for the Viewcontroller class
//See:  http://www.sdltutorials.com/sdl-tutorial-basics/
//		http://www.sdltutorials.com/sdl-opengl-tutorial-basics/
//		http://stackoverflow.com/questions/13826150/sdl2-opengl3-how-to-initialize-sdl-inside-a-function
//for many more details on how to write an OpenGL program using SDL.  You might also want to go to these 
//pages which will link you to other tutorials on how to do stuff with SDL.
//Be warned, however, that a lot of the tutorials describe SDL 1.2, but we will be using SDL 2 in this course.
//
//Specific to this lab, I found some helpful information on the following pages for centering the mouse in SDL 2 
//and trapping it in the window (i.e. you can't move the mouse outside the window)
//	http://stackoverflow.com/questions/10492143/sdl2-mouse-grab-is-not-working
//	http://gamedev.stackexchange.com/questions/33519/trap-mouse-in-sdl
//	http://www.ginkgobitter.org/sdl/?SDL_ShowCursor
//
//A big change in this class is that the user now moves around the scene using a traditional
//first person controller. Movement is controlled with wasd and view direction is changed with the mouse.
//The program now ends when the user presses the <Esc> key.

//The sound effects were downloaded from the following websites 
//foodsteps http://www.pacdv.com/sounds/people_sounds.html
//background http://www.brainybetty.com/soundsforpowerpoint2.htm
//collected http://www.superluigibros.com/super-mario-rpg-sound-effects-wav
//gunshots http://www.mediacollege.com/downloads/sound-effects/weapons/
//applause  http://www.soundjay.com/applause-sounds-1.html
//boo  http://www.wavsource.com/sfx/sfx.htm
#include <SDL.h>
#include <SDL_mixer.h>
#include "viewcontroller.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

const int WINDOWWIDTH = 1280;
const int WINDOWHEIGHT = 720;
const double PI = 3.14159;

Viewcontroller::Viewcontroller()
{
	quit = false;
	window = 0;
	ogl4context = 0;

	view_matrix = mat4(1.0);

	moveForward = 0.0;
	moveSideways = 0.0;
	MOVEANGLE = 3.0*PI/4.0;
	LOOKANGLE = 0.0;
	baseX = WINDOWWIDTH / 2.0;
	baseY = WINDOWHEIGHT / 2.0;
	eye = vec3(10.0, 0.0, -10.0);
	//eye = vec3(0.0, 8.0, 0.0);
	up = vec3(0.0, 1.0, 0.0);
		
	bossPos = vec3(-8.0, 0.0, 8.0);
	cube1Collected = false;
	cube2Collected = false;
	cube3Collected = false;
	cube4Collected = false;
	start = false;
	begin = false;
	end = false;
	win = false;
	updateLookAt();  //aim will be calculated from the initial values of eye and MOVEANGLE
}

//Initializes SDL, GLEW, and OpenGL
bool Viewcontroller::init()
{

	//First initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Failed to initialize SDL." << endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	if ((window = SDL_CreateWindow("Lab 12 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWWIDTH, WINDOWHEIGHT, SDL_WINDOW_OPENGL)) == NULL)
	{
		cout << "Failed to create window." << endl;
		return false;
	}
	ogl4context = SDL_GL_CreateContext(window);
	SDL_ShowCursor(0);  //Hide the mouse cursor

	//Initialize the Model that you want to render
	if (theWorld.init() == false)  //OpenGL initialization is done in the Model class
	{
		cout << "Failed to initialize theWorld." << endl;
		return false;
	}


	//Now then, we initialize our sound effects using SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
		return false;
	if ((footsteps = Mix_LoadWAV("sounds\\footsteps2.wav")) == NULL) //footsteps
	{
		cout << "FAILED to load footsteps.wav" << endl;
		return false;
	}
	if ((background = Mix_LoadWAV("sounds\\DesiJourney.wav")) == NULL) //background
	{
		cout << "FAILED to load DesiJourney.wav" << endl;
		return false;
	}
	if ((collected = Mix_LoadWAV("sounds\\collected.wav")) == NULL) //coin collected
	{
		cout << "FAILED to load collected.wav" << endl;
		return false;
	}
	if ((gunshot = Mix_LoadWAV("sounds\\gunshot.wav")) == NULL) //gunshots
	{
		cout << "FAILED to load gunshot.wav" << endl;
		return false;
	}
	if ((applause = Mix_LoadWAV("sounds\\applause.wav")) == NULL) //applause
	{
		cout << "FAILED to load applause.wav" << endl;
		return false;
	}
	if ((boo = Mix_LoadWAV("sounds\\boo.wav")) == NULL) //booing 
	{
		cout << "FAILED to load boo.wav" << endl;
		return false;
	}
	//background music 
	Mix_PlayChannel(-1, background, -1); //Plays forever and ever until the game ends 
	Mix_VolumeChunk(background, 3);
	Mix_VolumeChunk(footsteps, 20);
	Mix_VolumeChunk(collected, 20);
	Mix_VolumeChunk(gunshot, 40);
	return true;  //Everything got initialized
}

//Display what we want to see in the graphics window
void Viewcontroller::display()
{
	view_matrix = lookAt(eye, aim, up);  //calculate the view orientation matrix

	theWorld.draw(view_matrix, eye, MOVEANGLE, begin, win, end);

	SDL_GL_SwapWindow(window);
}


bool Viewcontroller::handleEvents(SDL_Event *theEvent)
{
	switch(theEvent->type)
	{
		case SDL_QUIT:  //User pressed the escape key
		{
			return true;  //force program to quit
		}
		case SDL_KEYDOWN:
		{
			if (theEvent->key.keysym.sym == SDLK_ESCAPE)  //the <Esc> key
			{
				return true;  //force game to quit
			}
			else if (theEvent->key.keysym.sym == SDLK_UP || theEvent->key.keysym.sym == SDLK_w && begin == true && end == false)
			{
				Mix_PlayChannel(-1, footsteps, 0);
					moveForward = 0.075;
			}
			else if (theEvent->key.keysym.sym == SDLK_LEFT || theEvent->key.keysym.sym == SDLK_a&& begin == true && end == false)
			{
				Mix_PlayChannel(-1, footsteps, 0);
					moveSideways = -0.075;
			}
			else if (theEvent->key.keysym.sym == SDLK_RIGHT || theEvent->key.keysym.sym == SDLK_d&& begin == true && end == false)
			{
				Mix_PlayChannel(-1, footsteps, 0);
					moveSideways = 0.075;
			}
			else if (theEvent->key.keysym.sym == SDLK_DOWN || theEvent->key.keysym.sym == SDLK_s&& begin == true && end == false)
			{
				Mix_PlayChannel(-1, footsteps, 0);
					moveForward = -0.075;
			}
			else if (theEvent->key.keysym.sym == SDLK_SPACE)
			{ 
				begin = true;
			}
			break;
		}
		case SDL_KEYUP:
		{
			if (theEvent->key.keysym.sym == SDLK_UP || theEvent->key.keysym.sym == SDLK_DOWN || theEvent->key.keysym.sym == SDLK_w || theEvent->key.keysym.sym == SDLK_s)
			{
				moveForward = 0;
			}
			else if (theEvent->key.keysym.sym == SDLK_LEFT || theEvent->key.keysym.sym == SDLK_RIGHT || theEvent->key.keysym.sym == SDLK_a || theEvent->key.keysym.sym == SDLK_d)
			{
				moveSideways = 0;
			}
			break;
		}
		case SDL_MOUSEMOTION:
		{
			const double MOUSE_SENSITIVITY_X = .01;
			const double MOUSE_SENSITIVITY_Y = .01;
			
			MOVEANGLE += (theEvent->button.x - baseX) * MOUSE_SENSITIVITY_X;
			LOOKANGLE += -(theEvent->button.y - baseY) * MOUSE_SENSITIVITY_Y;

			SDL_WarpMouseInWindow(window, baseX, baseY);  //re-center the mouse cursor
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			Mix_PlayChannel(-1, gunshot, 0);
			break;
		}
			
	} //end the switch
	return false;  //the program should not end
}

void Viewcontroller::updateLookAt()
{
	temp = eye;

	//Add movement forward and backward
	eye[0] += cos(MOVEANGLE)*moveForward;
	eye[2] += sin(MOVEANGLE)*moveForward;

	//Add movement to the left and right
	eye[0] += cos(MOVEANGLE + PI / 2.0)*moveSideways;
	eye[2] += sin(MOVEANGLE + PI / 2.0)*moveSideways;

	if (eye[2] < -10.50 || eye[2] > 10.50 || eye[0] < -10.50 || eye[0] > 10.50) //if out of bounds
	{
		eye = temp;
	}
	if (eye[2] > -8.750 && eye[2] < 8.750 && eye[0] > -8.750 && eye[0] < 8.750) //if in the grid
	{
		start = true;
	}
	//Adjust the aim position from the new eye position
	aim[0] = eye[0] + cos(MOVEANGLE);
	aim[1] = eye[1] + LOOKANGLE;
	aim[2] = eye[2] + sin(MOVEANGLE);
}

void Viewcontroller::run()
{
	if (init() == false)  //This method (defined above) sets up OpenGL, SDL, and GLEW
	{
		cout << "Program failed to initialize ... exiting." << endl;
		return;
	}

	SDL_Event events;  //Makes an SDL_Events object that we can use to handle events

	const int UPDATE_FREQUENCY = 10; //update the frame every 10 milliseconds
	long currentTime, startTime = clock();
	SDL_WarpMouseInWindow(window, baseX, baseY);  //Center the mouse cursor
	do
	{
		display();  //This method (defined above) draws whatever we have defined
		while (SDL_PollEvent(&events)) //keep processing the events as long as there are events to process
		{
			quit = handleEvents(&events);
		}

		currentTime = clock();
		if (currentTime - startTime > UPDATE_FREQUENCY)
		{
			if (start == true && end == false)
			{
				if (bossPos[2] - eye[2] > 0.01) //move down
					bossPos[2] -= 0.05;
				else if (bossPos[2] - eye[2] < 0.01) //move up
					bossPos[2] += 0.05;
				if (bossPos[0] - eye[0] < 0.01) //move left
					bossPos[0] += 0.05;
				else if (bossPos[0] - eye[0] > 0.01) //move right
					bossPos[0] -= 0.05;
			}
			updateLookAt();
			theWorld.updateWorld(bossPos);

			startTime = currentTime;

		} 
		
		if (abs(eye[0] - 4) < 0.5 && abs(eye[2] + 4) < 0.5) //At The position of Cube1
		{
			if (!cube1Collected) //if not collected, play the sound effect
				Mix_PlayChannel(-1, collected, 0);
			cube1Collected = true;
		}
		else if (abs(eye[0] + 4) < 0.5 && abs(eye[2] + 4) < 0.5) // At The position of Cube2
		{
			if (!cube2Collected) //if not collected, play the sound effect
				Mix_PlayChannel(-1, collected, 0);
			cube2Collected = true;
		}
		else if (abs(eye[0] + 4) < 0.5 && abs(eye[2] - 4) < 0.5) // At The position of Cube3
		{
			if (!cube3Collected) //if not collected, play the sound effect
				Mix_PlayChannel(-1, collected, 0);
			
			cube3Collected = true;
		}
		else if (abs(eye[0] - 4) < 0.5 && abs(eye[2] - 4) < 0.5) // At The position of Cube4
		{
			if (!cube4Collected) //if not collected, play the sound effect
				Mix_PlayChannel(-1, collected, 0);
			cube4Collected = true;
		}


		if (cube1Collected == true && cube2Collected == true && cube3Collected == true && cube4Collected == true) // if win
		{
			win = true;
			end = true;
			Mix_PlayChannel(-1, applause, -1);
			Mix_VolumeChunk(applause, 30);
		}
		if (abs(bossPos[0] - eye[0]) < 0.1 && abs(bossPos[2] - eye[2]) < 0.1) // if lose
		{
			win = false;
			end = true;
			Mix_PlayChannel(-1, boo, -1);
			Mix_VolumeChunk(boo, 30);
		}
	} while (!quit); //run until "quit" is true (i.e. user presses the <Esc> key

	SDL_GL_DeleteContext(ogl4context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}