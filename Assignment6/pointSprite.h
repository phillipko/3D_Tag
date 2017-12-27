//FileName:		pointSprite.h
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn


#ifndef POINTSPRITE_H
#define POINTSPRITE_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <gl/GLU.h>
#include <string>
#include "transparentTexModel.h"
using namespace std;

class PointSprite : public TransparentTexModel
{
public:
	//We'll need to redefine how these two methods work for this class
	virtual bool init(string texFileName, const GLfloat *c = 0);
	virtual void draw();
};

#endif