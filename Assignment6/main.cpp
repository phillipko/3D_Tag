//Comp 153 Assignment 6
//FileName:		main.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		This file defines the main() function for Lab 12.
//Movement is controller with wasd and view direction is changed with the mouse.
//The program ends when the user presses the <Esc> key

//Extra Credit Request
//1. In my program, there are six sound effects
	//a. When the player moves, footsteps will be heard.
	//b. There is a background music
	//c. When the player collects a cube, there will be a sound which notifies him/her
	//d. When the player clicks his/her mouse, a gunshot will be heard.
	//e. If the player wins, applause will be heard
	//f. If the player loses, booing will be heard

//2. This is a game!
	//This game is same as the game in assignment 2 except that this one is three dimentional and has more features
	//The player wins if all cubes are collected
	//The player loses if he/she is captured by the monster
#include "viewcontroller.h"
#include <stdlib.h>

int main(int argc, char *argv[])  //main() must take these parameters when using SDL
{
	Viewcontroller vc;

	vc.run();

	//system("pause");
	return 0;
}