//FileName:		light_tex.vert
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn


//The texture mapping code is adapted from Examples 6.8  and 6.27 in the OpenGL Programming Guide (8th edition)
//The rest of the code is based on Example 7.8 on pages 377 and 378 of the OpenGL Programming Guide (8th edition)
//with some tweaks to support shininess as a vertex property and to allow the eye position to move around the scene.
#version 420 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in float vertexShininess;
layout(location = 4) in vec2 in_tex_coord;  //texture coodinates are now being passed in as vertex attributes

out vec4 position; //position of the vertex in "eye" coordinates
out vec4 color;
out vec3 normal; //orientation of the normal in "eye" coordinates
out float shininess;
out vec2 vs_tex_coord; //texture coordinate for the vertex shader

layout(binding = 35,std140) uniform Matrices
{
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
};

void main()
{
	position = view_matrix * model_matrix * vertexPosition;
	color = vertexColor;
	vec4 n = view_matrix * model_matrix * vec4(vertexNormal, 0.0); //Assumes only isometric scaling
	normal = normalize(vec3(n.x, n.y, n.z));
	shininess = vertexShininess;
	vs_tex_coord = in_tex_coord;  //copy the texture coordinate passed into this shader to an out variable

	vec4 pos = projection_matrix * position;
	
	
	gl_PointSize = (1.0 - pos.z / pos.w) * 1000;  //Set the point size for the Point Sprites
	//gl_Position = vertexPosition;
	gl_Position = pos;
}