//FileName:		cube.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		To define the model that we want to render in our OpenGL program.

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "cube.h"
#include <iostream>
#include <string>
using namespace std;

Cube::Cube()
{
	VAO = 0;
	initialized = false;
}

//init() does all of the OpenGL initialization for your model(s)
bool Cube::init()
{
	glGenVertexArrays(1, &VAO);  //Create one vertex array object
	glBindVertexArray(VAO);

	//front face
	vertices[0][0] = -1.0; //x value
	vertices[0][1] = 1.0; //y value
	vertices[0][2] = 1.0; //z value
	vertices[1][0] = -1.0; //x value
	vertices[1][1] = -1.0; //y value
	vertices[1][2] = 1.0; //z value
	vertices[2][0] = 1.0; //x value
	vertices[2][1] = 1.0; //y value
	vertices[2][2] = 1.0; //z value
	vertices[3][0] = 1.0; //x value
	vertices[3][1] = -1.0; //y value
	vertices[3][2] = 1.0; //z value

	//two more vertices for right face
	vertices[4][0] = 1.0; //x value
	vertices[4][1] = 1.0; //y value
	vertices[4][2] = -1.0; //z value
	vertices[5][0] = 1.0; //x value
	vertices[5][1] = -1.0; //y value
	vertices[5][2] = -1.0; //z value

	//two more vertices for back face
	vertices[6][0] = -1.0; //x value
	vertices[6][1] = 1.0; //y value
	vertices[6][2] = -1.0; //z value
	vertices[7][0] = -1.0; //x value
	vertices[7][1] = -1.0; //y value
	vertices[7][2] = -1.0; //z value

	//Repeat the first two vertices so that we can draw with a GL_TRIANGLE_STRIP
	vertices[8][0] = -1.0; //x value
	vertices[8][1] = 1.0; //y value
	vertices[8][2] = 1.0; //z value
	vertices[9][0] = -1.0; //x value
	vertices[9][1] = -1.0; //y value
	vertices[9][2] = 1.0; //z value

	//Four vertices for the top
	vertices[10][0] = -1.0; //x value
	vertices[10][1] = 1.0; //y value
	vertices[10][2] = -1.0; //z value
	vertices[11][0] = -1.0; //x value
	vertices[11][1] = 1.0; //y value
	vertices[11][2] = 1.0; //z value
	vertices[12][0] = 1.0; //x value
	vertices[12][1] = 1.0; //y value
	vertices[12][2] = -1.0; //z value
	vertices[13][0] = 1.0; //x value
	vertices[13][1] = 1.0; //y value
	vertices[13][2] = 1.0; //z value

	//Four vertices for the top
	vertices[14][0] = -1.0; //x value
	vertices[14][1] = -1.0; //y value
	vertices[14][2] = 1.0; //z value
	vertices[15][0] = -1.0; //x value
	vertices[15][1] = -1.0; //y value
	vertices[15][2] = -1.0; //z value
	vertices[16][0] = 1.0; //x value
	vertices[16][1] = -1.0; //y value
	vertices[16][2] = 1.0; //z value
	vertices[17][0] = 1.0; //x value
	vertices[17][1] = -1.0; //y value
	vertices[17][2] = -1.0; //z value

	glGenBuffers(1, &Buffer); //Create a buffer objects for vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);  //Buffers[0] will be the position for each vertex
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(0);  //position is attribute location 0

	//NOTE: Since many vertices will share the values for color and surface normal, 
	//we'll set those with static vertex attributes in the draw() method.

	initialized = true;
	return true;  //Everything got initialized
}

//draw() explains how to render your model
void Cube::draw()
{
	if (initialized == false)
	{
		cout << "ERROR: Cannot render a Cube object before it has been initialized." << endl;
		return;
	}
	glBindVertexArray(VAO);

	//First set the shininess in attribute location 3
	glVertexAttrib1f(3, 2.0);	//All sides will have the same "shininess".  This might seem
								//counterintuitive, but the smaller this number the more
								//noticable the specular highlights will be.
	//Make front face red
	glVertexAttrib4f(1, 1.0, 0.0, 0.0, 1.0);  //color is attribute location 1
	glVertexAttrib3f(2, 0.0, 0.0, 1.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Make right face green
	glVertexAttrib4f(1, 0.0, 1.0, 0.0, 1.0);
	glVertexAttrib3f(2, 1.0, 0.0, 0.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);

	//Make back face blue
	glVertexAttrib4f(1, 0.0, 0.0, 1.0, 1.0);
	glVertexAttrib3f(2, 0.0, 0.0, -1.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	//Make left face yellow
	glVertexAttrib4f(1, 1.0, 1.0, 0.0, 1.0);
	glVertexAttrib3f(2, -1.0, 0.0, 0.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_STRIP, 6, 4);

	//Make top face cyan
	glVertexAttrib4f(1, 0.0, 1.0, 1.0, 1.0);
	glVertexAttrib3f(2, 0.0, 1.0, 0.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_STRIP, 10, 4);

	//Make bottom face pink
	glVertexAttrib4f(1, 1.0, 0.0, 1.0, 1.0);
	glVertexAttrib3f(2, 0.0, -1.0, 0.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_STRIP, 14, 4);
}