//FileName:		sphere.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		To define the model that we want to render in our OpenGL program.

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "sphere.h"
#include <iostream>
#include <string>
using namespace std;

const GLfloat pi = 3.1415926535897932384626;

Sphere::Sphere()
{
	VAO = 0;
	initialized = false;
}

//init() does all of the OpenGL initialization for your model(s)
bool Sphere::init()
{
	glGenVertexArrays(1, &VAO);  //Create one vertex array object
	glBindVertexArray(VAO);

	//A sphere
	int vertexNum = 0;

	GLfloat phi = 0;
	GLfloat theta = 0;
	//front face
	vertices[0][0] = 0.0; //x value
	vertices[0][1] = 0.0; //y value
	vertices[0][2] = 0.0; //z value 

	for (vertexNum = 1; vertexNum < 10000; vertexNum++)
	{
		 
		vertices[vertexNum][0] = sin(phi)*cos(theta);
		vertices[vertexNum][1] = sin(theta)*sin(phi);
		vertices[vertexNum][2] = cos(phi);
		 
		phi += 0.1;
		theta += pi / (10000 - 1);
	}
	vertices[10000][0] = 0.0;
	vertices[10000][1] = 0.0;
	vertices[10000][2] = 0.0;
	vertices[10001][0] = 0.0;
	vertices[10001][1] = 10.0;
	vertices[10001][2] = 0.0;
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
void Sphere::draw()
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
	//A sphere
	glVertexAttrib4f(1, 1.0, 0.0, 0.0, 1.0); //color is attribute location 1
	glVertexAttrib3f(2, 0.0, 0.0, 1.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5000);

	glVertexAttrib4f(1, 0.0, 0.0, 0.0, 0.0); //color is attribute location 1
	glVertexAttrib3f(2, 1.0, 0.0, 0.0);  //normal is attribute location 2
	glDrawArrays(GL_TRIANGLE_FAN, 5000, 5000);

	glVertexAttrib4f(1, 0.0, 0.0, 0.0, 0.0); //color is attribute location 1
	glVertexAttrib3f(2, 1.0, 0.0, 0.0);  //normal is attribute location 2
	glDrawArrays(GL_LINES, 10000, 2);
}