//FileName:		brick.vert
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn


//This vertex shader extends our previous ones and implements Example 8.3 on pages 420 and 421 of the OpenGL 
//Programming Guide (8th edition) to build a procedural brick texture.  Note that to get the texture to orient 
//correctly on all sides of the cube I had to set "MCposition" based on the direction of the surface normal.

#version 420 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in float vertexShininess;

out vec4 pos; //position of the vertex in "eye" coordinates
out vec4 fColor;
out vec3 normal; //orientation of the normal in "eye" coordinates
out float shininess;
out vec2 MCposition;  //NEW out variable for this shader

layout(binding = 35,std140) uniform Matrices
{
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
};

void main()
{
	pos = view_matrix * model_matrix * vertexPosition;
	fColor = vertexColor;
	vec4 n = view_matrix * model_matrix * vec4(vertexNormal, 0.0); //Assumes only isometric scaling
	normal = normalize(vec3(n.x, n.y, n.z));
	shininess = vertexShininess;

	//Set MCposition based on the direction of the normal vector
	if (vertexNormal.z > 0.1 || vertexNormal.z <= -0.1)
		MCposition = vertexPosition.xy;
	else if (vertexNormal.y > 0.1 || vertexNormal.y < -0.1)
		MCposition = vertexPosition.xz;
	else if (vertexNormal.x > 0.1 || vertexNormal.x < -0.1)
		MCposition = vertexPosition.zy;

	gl_Position = projection_matrix * pos;
}