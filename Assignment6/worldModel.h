//FileName:		worldModel.h
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		This file defines the header for the WorldModel class.
//This Model class differs from ones in previous labs in that it is responsible for loading the shaders and 
//rendering all of the objects in their proper places. Basically, OpenGL and GLEW stuff goes in this class.
//SDL goes in the Viewcontroller. The Viewcontroller then passes the view_matrix to the draw() method of this class.

#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <glew.h> //glew.h is supposed to be included before gl.h. To be safe, you can just include glew.h instead
#include <glm.hpp>
#include "quad.h"
#include "transparentTexModel.h"  
#include "pointSprite.h"
#include "shaderModel.h"
#include "cube.h"
#include "rectangle.h"
#include "sphere.h"
#include "groundStripes.h"  
using namespace glm;

class WorldModel
{
protected:
	bool initialized;

	GLuint program;
	GLuint program3; 
	//Variables for matrix manipulations
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
	GLuint matrixLoc;
	vec3 eye;
	vec3 bossPos;
	//Define the objects we want to render in our scene
 
	TransparentTexModel gun;
	TransparentTexModel north;
	TransparentTexModel m;
	TransparentTexModel graffiti;
	TransparentTexModel welcome;
	TransparentTexModel winMessage;
	TransparentTexModel loseMessage;
	PointSprite monster;
	ShaderModel <Quad> ground;
	ShaderModel <Cube> brick;
	ShaderModel <Cube> cube; 
	ShaderModel <GroundStripes> stripes;
	ShaderModel <Sphere> ball;


	bool drawCube1;
	bool drawCube2;
	bool drawCube3;
	bool drawCube4;

	 
	GLuint brickNoiseTexID;
	GLint numTexLoc;
	float cube_rot_angle;

	 

public:
	WorldModel();

	void setUpLights();
	void setUpBrickTex();

	void updateMatrices();
	void updateWorld(vec3 bossPosition);

	bool initGLEW();
	bool init();  //initializes the model
	void draw(mat4 view_matrix, vec3 eye, double MOVEANGLE, bool begin, bool win, bool end);  //renders the model
};

#endif