//FileName:		cube.h
//Programmer:	Phillip Ko
//Date:			11/14/2014
//Purpose:		This file defines the header for a cube that is centered at the origin
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn
#ifndef CUBE_H
#define CUBE_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead

class Cube
{
private:
	bool initialized;

	GLfloat vertices[18][3];  //each vertex has an x, y, and z value
	GLfloat color[4];
	GLfloat normal[3];
	GLuint VAO;
	GLuint Buffer;  //We'll need a buffer for the vertex positions

public:
	Cube();

	bool init();
	void draw();
};

#endif