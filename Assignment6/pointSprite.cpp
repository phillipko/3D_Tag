//FileName:		pointSprite.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		Sets up a point sprite (see the OpenGL Programming Guide, 8th edition, pages 346-351)

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "pointSprite.h"
#include "texture.h"
#include <iostream>
#include <string>
using namespace std;

//init() does all of the OpenGL initialization for your model(s)
bool PointSprite::init(string texFileName, const GLfloat *c)
{
	glGenVertexArrays(1, &VAO);  //Create one vertex array object
	glBindVertexArray(VAO);

	vertices[0][0] = 0.0;  //x value for vertex 0
	vertices[0][1] = 0.0;  //y value for vertex 0
	vertices[0][2] = 0.0; //z value for vertex 0

	glGenBuffers(1, &Buffer); //Create a buffer object for vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);  //Buffers[0] will be the position for each vertex
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Do the shader plumbing here for this buffer
	glEnableVertexAttribArray(0);

	defineVerticeColor(c); //Call the method inherited from Quad to set up the vertices' color

	//Now we need to load our texture
	initTexture(texFileName);  //call the method inherited from TransparentTexModel
	
	//Apparently Point Sprites and not enabled for the Vertex and Fragment shaders by default.
	//You need to make sure you enable them.
	glPointParameterfEXT(GL_POINT_SIZE_MIN, 1.0f);
	glPointParameterfEXT(GL_POINT_SIZE_MAX, 128.0f);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	//glPointSize(32);  //The line above tells OpenGL that we are setting the point size in the vertex shader

	initialized = true;
	return true;  //Everything got initialized
}

//draw() explains how to render your model
void PointSprite::draw()
{
	if (initialized == false)
	{
		cout << "ERROR: Cannot render a PointSprite object before it has been initialized." << endl;
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

	glDrawArrays(GL_POINTS, 0, 1);

	glDisable(GL_ALPHA_TEST);
}