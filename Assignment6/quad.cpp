//FileName:		quad.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		To define the model that we want to render in our OpenGL program.

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "quad.h"
#include <iostream>
#include <string>
using namespace std;

Quad::Quad()
{
	VAO = 0;
	initialized = false;
}

void Quad::defineVertexPositions()
{
	float z = 0.0;
	//Vertex positions
	vertices[0][0] = -1.0;  //x value for vertex 0
	vertices[0][1] = 1.0;  //y value for vertex 0
	vertices[0][2] = z; //z value for vertex 0
	vertices[1][0] = -1.0;  //x value for vertex 1
	vertices[1][1] = -1.0;  //y value for vertex 1
	vertices[1][2] = z; //z value for vertex 1
	vertices[2][0] = 1.0;  //x value for vertex 2
	vertices[2][1] = 1.0;  //y value for vertex 2
	vertices[2][2] = z; //z value for vertex 2
	vertices[3][0] = 1.0;  //x value for vertex 3
	vertices[3][1] = -1.0;  //y value for vertex 3
	vertices[3][2] = z; //z value for vertex 3
}

void Quad::defineVerticeColor(const GLfloat *c)
{
	for (int i = 0; i < 4; i++)
	{
		if (c != 0)
		{
			color[i] = c[i];
		}
		else
		{
			color[i] = 1.0;  //default color will be white
		}
	}
}

void Quad::printVerticeColor()
{
	cout << color[0] << "," << color[1] << "," << color[1] << endl;
}

//init() does all of the OpenGL initialization for your model(s)
bool Quad::init(const GLfloat *c)
{
	glGenVertexArrays(1, &VAO);  //Create one vertex array object
	glBindVertexArray(VAO);

	defineVertexPositions();  //Call the method up above to set up the vertex positions

	glGenBuffers(1, &Buffer); //Create a buffer objects for vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);  //Buffers[0] will be the position for each vertex
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(0); //vertex position is attribute location 0

	defineVerticeColor(c); //Call the method up above to set up the vertices' color

	initialized = true;
	return true;  //Everything got initialized
}

//draw() explains how to render your model
void Quad::draw()
{
	if (initialized == false)
	{
		cout << "ERROR: Cannot render a Quad object before it has been initialized." << endl;
		return;
	}

	glBindVertexArray(VAO);

	glVertexAttrib4f(1, color[0], color[1], color[2], color[3]);  //color is attribute location 1
	glVertexAttrib3f(2, 0.0, 0.0, 1.0);  //normal is attribute location 2
	glVertexAttrib1f(3, 2.0); //shininess is attribute location 3
	glVertexAttrib2f(4, 0.0, 0.0); //texture coordinates are location 4

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}