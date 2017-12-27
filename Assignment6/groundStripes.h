//FileName:		groundStripes.h
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

#ifndef GROUNDSTRIPES_H
#define GROUNDSTRIPES_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead

class GroundStripes
{
private:
	bool initialized;

	GLfloat vertices[40][3];  //each vertex has an x, y, and z value
	GLfloat color[4];
	GLfloat normal[3];
	GLuint VAO;
	GLuint Buffer;  //We'll need a buffer for the vertex positions

public:
	GroundStripes();

	bool init();
	void draw();
};

#endif