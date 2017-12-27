//FileName:		rectangle.h
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		This file defines the header for a rectangle class.

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <gl/GLU.h>
#include <string>
#include "quad.h"
using namespace std;

class Rectangle : public Quad
{
public:
	virtual void defineVertexPositions();
};

#endif