//FileName:		rectangle.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		To define the model that we want to render in our OpenGL program.

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "rectangle.h"
#include <iostream>
#include <string>
using namespace std;

void Rectangle::defineVertexPositions()
{
	float z = 0.0;
	//Vertex positions
	vertices[0][0] = -0.5;  //x value for vertex 0
	vertices[0][1] = 0.5;  //y value for vertex 0
	vertices[0][2] = z; //z value for vertex 0
	vertices[1][0] = -0.5;  //x value for vertex 1
	vertices[1][1] = -0.5;  //y value for vertex 1
	vertices[1][2] = z; //z value for vertex 1
	vertices[2][0] = 0.5;  //x value for vertex 2
	vertices[2][1] = 0.5;  //y value for vertex 2
	vertices[2][2] = z; //z value for vertex 2
	vertices[3][0] = 0.5;  //x value for vertex 3
	vertices[3][1] = -0.5;  //y value for vertex 3
	vertices[3][2] = z; //z value for vertex 3
}