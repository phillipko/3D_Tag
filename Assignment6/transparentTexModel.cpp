//FileName:		transparentTexModel.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		To define the model that we want to render in our OpenGL program.

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "transparentTexmodel.h"
#include "texture.h"
#include <iostream>
#include <string>
using namespace std;

void TransparentTexModel::initTexture(string texFileName)
{
	Texture t(texFileName, true, addAlpha, FINDBLACK); cout << endl;

	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, t.width(), t.height());
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, t.width(), t.height(), GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glAlphaFunc(GL_GREATER, 0);
}

void TransparentTexModel::draw() //This draw method enables an Alpha Test.  If the image has an 
{								 //alpha of zero then that part of the image is made invisible.
	if (initialized == false)
	{
		cout << "ERROR: Cannot render a TransparentTexModel object before it has been initialized." << endl;
		return;
	}
	
	glEnable(GL_ALPHA_TEST);
	
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID); //Bind the texture with the ID of texID to this VAO
	//NOTE that we need to make sure that the corect texture is bound for the appropriate object before every draw.
	//Othwerise, the last texture loaded will be applied to every object in the scene 

	glVertexAttrib4f(1, color[0], color[1], color[2], color[3]);  //color is attribute location 1
	glVertexAttrib3f(2, 0.0, 0.0, 1.0);  //normal is attribute location 2
	glVertexAttrib1f(3, 2.0); //shininess is attribute location 3

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisable(GL_ALPHA_TEST);
}