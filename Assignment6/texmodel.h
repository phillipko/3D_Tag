//FileName:		texmodel.h
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		This file defines the header for the TexModel class.

#ifndef TEXMODEL_H
#define TEXMODEL_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include <gl/GLU.h>
#include <string>
#include "quad.h"
using namespace std;

class TexModel : public Quad
{
	protected:
		GLfloat tex_coords[4][2];
		GLuint Buffers[2];  //We'll need one buffer for the vertex positions and one for the texture coordinates
		GLuint texID;

	public:
		TexModel();

		virtual void buildMipMaps();

		virtual void defineTexCoords();
		virtual bool init(string texFileName, const GLfloat *c = 0);  //initializes the model and must take the file name of the texture
		virtual void initTexture(string texFileName);
		virtual void draw();  //renders the model
};

#endif