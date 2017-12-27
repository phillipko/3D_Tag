//FileName:		groundStripes.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		To define the model that we want to render in our OpenGL program.

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "groundStripes.h"
#include <iostream>
#include <string>
using namespace std;

GroundStripes::GroundStripes()
{
	VAO = 0;
	initialized = false;
}

//init() does all of the OpenGL initialization for your model(s)
bool GroundStripes::init()
{
	glGenVertexArrays(1, &VAO);  //Create one vertex array object
	glBindVertexArray(VAO);
	vertices[0][0] = -0.8; //x value
	vertices[0][1] = 0.8; //y value
	vertices[0][2] = 0.0; //z value
	vertices[1][0] = -0.8; //x value
	vertices[1][1] = -0.8; //y value
	vertices[1][2] = 0.0; //z value
	vertices[2][0] = 0.8; //x value
	vertices[2][1] = 0.8; //y value
	vertices[2][2] = 0.0; //z value
	vertices[3][0] = 0.8; //x value
	vertices[3][1] = -0.8; //y value
	vertices[3][2] = 0.0; //z value
	float xPos = -0.9;
	float yPos = -0.9;
	int temp = 0;
	//The following for loop draws our gameboard
	for (int vertexNum = 0; vertexNum < 40; vertexNum++)
	{
		//yPos = rand()%19 - 9;
		//cout << "vertexNum:" << vertexNum << endl;
		vertices[vertexNum][0] = xPos;  //x value for vertex
		vertices[vertexNum][1] = yPos;  //y value for vertex
		vertices[vertexNum][2] = 0.0;
		if (vertexNum % 2 == 0 && vertexNum < 19)
		{
			xPos += 1.8;
		}
		else if (vertexNum % 2 == 1 && vertexNum < 19)
		{
			xPos -= 1.8;
			yPos += 0.2;
		}
		if (vertexNum == 19)
		{
			xPos = -0.9;
			yPos = -0.9;
		}
		if (vertexNum % 2 == 0 && vertexNum > 19)
		{
			yPos += 1.8;
		}
		else if (vertexNum % 2 == 1 && vertexNum > 19)
		{
			yPos -= 1.8;
			xPos += 0.2;
		}
	}
	 
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
void GroundStripes::draw()
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
	glLineWidth(5.0);
	glVertexAttrib4f(1, 1.0, 1.0, 1.0, 1.0);  //color is attribute location 1
	glVertexAttrib3f(2, 0.0, 1.0, 0.0);  //normal is attribute location 2
	glDrawArrays(GL_LINES, 0, 40);
	 
}