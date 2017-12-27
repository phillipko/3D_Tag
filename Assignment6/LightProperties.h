//FileName:		LightProperties.h
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//ACKNOWLEDGEMENT: This is Example 7.7 discussed in the OpenGL Programming Guide (8th edition) on 
//page 376 with some tweaks to match better with the Phong Reflectance model we discussed in class.  
//I am also requiring that the programmer use the w value of position to indicate whether the light 
//is a point light (w = 1) or a directional (w = 0).

#ifndef LIGHTPROPERTIES_H
#define LIGHTPROPERTIES_H

#include <glm.hpp>
using namespace glm;

struct LightProperties
{
	vec4 color;			//color of light
	vec4 position;		//location of light if w = 1,
						//otherwise the direction towards the light
	
	//spotlight attributes (can only use if light is local, i.e. position.w = 1)
	vec4 spotLightValues;	//if spotLightValues.x > 0 then it is a spotLight
							//spotLightValues.y is spotCosCuttoff
							//spotLightValues.z is spotExponent
	vec4 spotConeDirection;  //direction that the spotlight is shinining

	//local light attenuation coefficients (position.w must be 1 to use these)
	GLfloat constantAttenuation;
	GLfloat linearAttenuation;
	GLfloat quadraticAttenuation;

	GLuint isEnabled;
};


#endif