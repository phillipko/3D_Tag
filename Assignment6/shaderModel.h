//FileName:		shaderModel.h
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		This file defines the header for a template ShaderModel class. 
//This template must be used with a class that implements a draw() method to render objects
//and an init() method to set up all of the OpenGL initialization for the object.
//This class also assumes that the loaded shaders use variables called: projectionMatrix,
//viewMatrix, and modelMatrix to do the matrix multiplication in the vertex shader.

#ifndef SHADERMODEL_H
#define SHADERMODEL_H

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "LoadShaders.h"
#include <glm.hpp>
using namespace glm;

template <class T>
class ShaderModel
{
protected:
	bool LOADED_SHADERS;

	GLuint program;

	T theObject;

public:
	ShaderModel()
	{
		program = 0;
	}

	ShaderModel(ShaderInfo shaders[])
	{
		program = 0;
		init(shaders);
	}

	GLuint getGLSLProgram() { return program; }
	T *getObject() { return &theObject; }

	bool init(ShaderInfo shaders[])
	{
		//Load the shaders using the LoadShaders() function written by the authors of the OpenGL Programming Guide
		if ((program = LoadShaders(shaders)) == 0)
		{
			cout << "Error Loading Shaders in the ShaderModel class." << endl;
			return false;
		}
		glUseProgram(program);
		LOADED_SHADERS = true;

		theObject.init();

		return true;
	}

	void draw()
	{
		if (LOADED_SHADERS == false)
		{
			cout << "Cannot draw an object of the Shader Model class without first loading the shaders." << endl;
			return;
		}

		glUseProgram(program);

		theObject.draw();
	}
};

#endif