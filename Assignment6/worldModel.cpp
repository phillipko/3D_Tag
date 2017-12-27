//FileName:		model.cpp
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//Purpose:		Define the methods for the World Model class.
//The init() method needs to set up OpenGL and GLEW and prepare all objects (and their shaders) to be rendered.
//The draw() method positions and renders all objects in the scene and activates the appropriate shader(s).

//The image was downloaded from this website
//gun.bmp http://i.ytimg.com/vi/cJPbH303YWk/maxresdefault.jpg

#include <glew.h>  //glew.h is supposed to be included before gl.h.  To be safe, you can just include glew.h instead
#include "LoadShaders.h"
#include "worldModel.h"
#include "LightProperties.h"
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
using namespace glm;
using namespace std;

WorldModel::WorldModel()
{ 
	cube_rot_angle = 0.0;
	model_matrix = mat4(1.0);
	view_matrix = mat4(1.0);
	projection_matrix = mat4(1.0);

	drawCube1 = true;
	drawCube2 = true;
	drawCube3 = true;
	drawCube4 = true;
	 
}

bool WorldModel::initGLEW()
{
	//Next initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
		return false;
	}

	//The following code was adapted from the OpenGL 4.0 Shading Language Cookbook, by David Wolff
	//to provide information about the hardware and supported versions of OpenGL and GLSL. 
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << "GL Vendor: " << vendor << endl;
	cout << "GL Renderer: " << renderer << endl;
	cout << "GL Version: " << version << endl;
	cout << "GLSL Version: " << glslVersion << endl << endl;

	return true;
}

void WorldModel::setUpLights()
{
	//IMPORTANT - If you change this structure in any way you need to change it in all fragment shader(s) as well!!!
	struct Lights
	{
		LightProperties lights[4];
		vec3 globalAmbientLight;
		int totalLights;
	} lightInfo;

	//Now, set up the lights for the scene
	lightInfo.totalLights = 4;
	lightInfo.globalAmbientLight = vec3(1.0, 1.0, 1.0);

	   
	lightInfo.lights[0].color = vec4(1.0, 1.0, 1.0, 1.0);
	lightInfo.lights[0].position = vec4(0.0, 1.0, 0.0, 0.0);  //directional light since w = 0
	//No attenuation for directional lights so we don't need to set those parameters
	lightInfo.lights[0].isEnabled = 1;

	lightInfo.lights[1].color = vec4(0.0, 0.5, 0.0, 1.0);
	lightInfo.lights[1].position = vec4(1.5, 10.0, 1.5, 1.0);  //positional light since w = 1
	lightInfo.lights[1].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[1].constantAttenuation = 2.0;
	lightInfo.lights[1].linearAttenuation = 0.0;
	lightInfo.lights[1].quadraticAttenuation = 0.0;
	lightInfo.lights[1].isEnabled = 1;

	lightInfo.lights[2].color = vec4(0.0, 0.0, 0.2, 1.0);
	lightInfo.lights[2].position = vec4(5.0, 10, 0.0, 1.0);  //positional light since w = 1
	lightInfo.lights[2].spotLightValues = vec4(0.0, 0.0, 0.0, 0.0);
	lightInfo.lights[2].constantAttenuation = 2.0;
	lightInfo.lights[2].linearAttenuation = 0.0;
	lightInfo.lights[2].quadraticAttenuation = 0.0;
	lightInfo.lights[2].isEnabled = 1;

	lightInfo.lights[3].color = vec4(1.0, 1.0, 1.0, 1.0);
	lightInfo.lights[3].position = vec4(-1.5, 5.0, 1.5, 1.0);  //positional light since w = 1
	lightInfo.lights[3].spotLightValues = vec4(1.0, 0.95, 4.0, 0.0);
		//If the first parameter to spotLightValues is > 0, then this is a spotlight
		//The second parameter to spotLightValues is the Spot Cosine Cutoff
		//The third parameter to spotLightValues is the Spotlight Exponent
		//The fourth parameter to spotLightValues is unused
	lightInfo.lights[3].spotConeDirection = vec4(-1.5, -1.0, 1.5, 0.0);
	lightInfo.lights[3].constantAttenuation = 0.5;
	lightInfo.lights[3].linearAttenuation = 0.0;
	lightInfo.lights[3].quadraticAttenuation = 0.0;
	lightInfo.lights[3].isEnabled = 1;
	
	

	//Pass the light info to the shaders in a Uniform Buffer Object.
	//This allows ALL shaders to be able to access the light information.
	GLuint lightsLoc;
	glGenBuffers(1, &lightsLoc);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsLoc);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lightInfo), &lightInfo, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsLoc); //The 0 needs to match the number used in the shaders for the lights
}

//This method updates the Matrices uniform buffer object with the values of the model, view, 
//and projection matrices making their values accessible to ALL of the shaders.
void WorldModel::updateMatrices()
{
	//IMPORTANT - If you change this structure in any way you need to change it in all the shaders as well!!!
	struct Matrices
	{
		mat4 model_matrix;
		mat4 view_matrix;
		mat4 projection_matrix;
	} matrices;

	matrices.model_matrix = model_matrix;
	matrices.projection_matrix = projection_matrix;
	matrices.view_matrix = view_matrix;
	
	//Pass the matrix info to the shaders in a Uniform Buffer Object.
	//This allows ALL shaders to be able to access the matrix information.
	glBufferData(GL_UNIFORM_BUFFER, sizeof(matrices), &matrices, GL_DYNAMIC_DRAW);//use GL_DYNAMIC_DRAW since it changes a lot
	glBindBufferBase(GL_UNIFORM_BUFFER, 35, matrixLoc);  //The 35 needs to match the number used in the shaders for the matrices
}

void WorldModel::setUpBrickTex()
{
	GLuint program2 = brick.getGLSLProgram();
	glUseProgram(program2);
	//Find and set the uniform shader variables for the brick's shaders (shaders2)
	GLint shaderLoc = glGetUniformLocation(program2, "BrickColor");
	glUniform3f(shaderLoc, 0.3, 0.1, 0.1);
	shaderLoc = glGetUniformLocation(program2, "MortarColor");
	glUniform3f(shaderLoc, 0.5, 0.5, 0.5);
	shaderLoc = glGetUniformLocation(program2, "BrickSize");
	glUniform2f(shaderLoc, 0.5, 0.2);
	shaderLoc = glGetUniformLocation(program2, "BrickPct");
	glUniform2f(shaderLoc, .95, .95);
	//The following code creates a "noise" texture to send to the fragment shader of the brick
	const int TexWidth = 400, TexHeight = 400;
	unsigned char NoiseTex[TexHeight][TexWidth][4];
	for (int r = 0; r < TexHeight; r++)
	{
		for (int c = 0; c < TexWidth; c++)
		{
			for (int i = 0; i < 4; i++)
			{
				NoiseTex[r][c][i] = (unsigned char)(rand() % 128) + 128;
			}
		}
	}
	glGenTextures(1, &brickNoiseTexID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickNoiseTexID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, TexWidth, TexHeight);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TexWidth, TexHeight, GL_RGBA, GL_UNSIGNED_BYTE, NoiseTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

bool WorldModel::init()
{
	if (initGLEW() == false)
	{
		return false;
	}

	//Do some OpenGL initialization
	glClearColor(0.7, 0.7, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	//Load the shaders using the LoadShaders() function written by the authors of the OpenGL Programming Guide
	ShaderInfo shaders1[] = {
			{ GL_VERTEX_SHADER, "shaders/light_tex.vert" },
			{ GL_FRAGMENT_SHADER, "shaders/light_tex.frag" },
			{ GL_NONE, NULL }
	};
	if ((program = LoadShaders(shaders1)) == 0)
	{
		cout << "Error Loading Shaders" << endl;
		return false;
	}
	glUseProgram(program);

	//Find the locations of the uniform variables in shaders1 (the primary shader)
	numTexLoc = glGetUniformLocation(program, "numTextures");	
	 
	//Now initialize all of our models that use shaders1. Note that I have moved this a little 
	//later in init() from previous labs so that we can pass "program" to the MultiTexModel object.
 
	welcome.init("images/welcome.bmp");
	winMessage.init("images/win.bmp");
	loseMessage.init("images/lose.bmp");
	monster.init("images/monster.bmp");
	gun.init("images/gun.bmp");
	north.init("images/north.bmp");
	m.init("images/monster.bmp");
	graffiti.init("images/graffiti.bmp");
	//Set up the brick object to use its own shaders
	ShaderInfo shaders2[] = {
			{ GL_VERTEX_SHADER, "shaders//brick.vert" },
			{ GL_FRAGMENT_SHADER, "shaders//brick.frag" },
			{ GL_NONE, NULL }
	};
	brick.init(shaders2);
	setUpBrickTex();

	
	//Set up the ground and cube objects to use the regular phong shaders (shaders4)
	ShaderInfo shaders4[] = {
			{ GL_VERTEX_SHADER, "shaders/phong.vert" },
			{ GL_FRAGMENT_SHADER, "shaders/phong.frag" },
			{ GL_NONE, NULL }
	};
	GLfloat ground_color[4] = { 0.1, 0.15, 0.1, 1.0 };
	//GLfloat ground_color[4] = { 0.1, 0.15, 1.0, 1.0 };
	ground.init(shaders4);
	((Quad *)ground.getObject())->defineVerticeColor(ground_color);
	cube.init(shaders4);
	stripes.init(shaders4); 
	ball.init(shaders4);
	

	//set up tex_2D

	ShaderInfo shaders6[] =
	{
		{ GL_VERTEX_SHADER, "shaders/tex_2D.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/tex_2D.frag" },
		{ GL_NONE }
	};
	
	
	if ((program3 = LoadShaders(shaders6)) == 0)
	{
		cout << "Error Loading Shaders" << endl;
		return false;
	}

	//Find the locations of the uniform variables in shader (the primary shader)
	//numTexLoc = glGetUniformLocation(program3, "numTextures");
	

	//Set up the uniform buffer objects that hold data that all of the shaders share. In this
	//application we have two uniform buffer objects: one for the lights and one for the matrices.
	setUpLights();  //The lights don't change as the program runs so we can set them here
	glGenBuffers(1, &matrixLoc);
	glBindBuffer(GL_UNIFORM_BUFFER, matrixLoc);
	//Note that updateMatrices() gets called in draw() so we really do not need to call it here.
	//We call updateMatrices() in draw() since the matrices can change for every frame.

	//Since the projection matrix will not change during the program we can calculate it here
	projection_matrix = frustum(-0.2f, 0.2f, -0.1f, 0.1f, 0.1f, 100.0f);

	initialized = true;
	return true;
}

void WorldModel::updateWorld(vec3 bossPosition)
{
	cube_rot_angle += 0.5;

	if (cube_rot_angle > 360.0) cube_rot_angle = 0.0;

	bossPos = bossPosition;
}

void WorldModel::draw(mat4 viewMatrixIn, vec3 eye, double MOVEANGLE, bool begin, bool win, bool end)
{
	//Check to see if cubes are collected
	if (abs(eye[0] - 4) < 0.5 && abs(eye[2] + 4) < 0.5)
		drawCube1 = false;
	if (abs(eye[0] + 4) < 0.5 && abs(eye[2] + 4) < 0.5)
		drawCube2 = false;
	if (abs(eye[0] + 4) < 0.5 && abs(eye[2] - 4) < 0.5)
		drawCube3 = false;
	if (abs(eye[0] - 4) < 0.5 && abs(eye[2] - 4) < 0.5)
		drawCube4 = false;
	view_matrix = viewMatrixIn;
	
	if (initialized == false)
	{
		cout << "ERROR: Cannot render a Model object before it has been initialized." << endl;
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 
	glUseProgram(program3); //Make the base shader active (in this case it is the light_tex shader1)
	glUniform1i(numTexLoc, 1);
	if (!begin) //If game not started, display the welcome message 
		welcome.draw();
	if (end == true && win == true) //if finished, and the player wins, Display the appropriate message 
		winMessage.draw();
	else if (end == true && win == false) //same idea 
		loseMessage.draw();

	
	gun.draw(); 
	
	
	glUseProgram(program); //Make the base shader active (in this case it is the light_tex shader1)
	glUniform1i(numTexLoc, 1); //set this uniform variable for the objects that have one texture
	
	// North East South West on the wall
	model_matrix = translate(mat4(1.0), vec3(0.0f, 5.0f, 10.9f));
	model_matrix = scale(model_matrix, vec3(10.0f, 6.0f, 6.0f));
	updateMatrices();
	north.draw();
	
	// Draw monster pictures on the wall
	model_matrix = translate(mat4(1.0), vec3(0.0f, 5.0f, -10.9f));
	model_matrix = scale(model_matrix, vec3(10.0f, 6.0f, 6.0f));
	updateMatrices();
	m.draw();

	model_matrix = translate(mat4(1.0), vec3(10.9f, 5.0f, 0.0f));
	model_matrix = rotate(model_matrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
	model_matrix = scale(model_matrix, vec3(10.0f, 6.0f, 6.0f));
	updateMatrices();
	m.draw();

	model_matrix = translate(mat4(1.0), vec3(-10.9f, 5.0f, 0.0f));
	model_matrix = rotate(model_matrix, 90.0f, vec3(0.0f, 1.0f, 0.0f));
	model_matrix = scale(model_matrix, vec3(10.0f, 6.0f, 6.0f));
	updateMatrices();
	graffiti.draw();

	glUniform1i(numTexLoc, 3); //set this uniform variable for the PointSprite objects

	//Draw the monster 
	model_matrix = translate(mat4(1.0), vec3(bossPos[0], 0.0f, bossPos[2])); //Position the sprite
	model_matrix = scale(model_matrix, vec3(10.0f, 10.0f, 10.0f));
	updateMatrices();
	monster.draw();

	
	//Render the objects that use their own shaders
	glBindTexture(GL_TEXTURE_2D, brickNoiseTexID); //Need to make the noise texture active for the brick
	model_matrix = translate(mat4(1.0), vec3(0.0f, 0.0f, 0.0f)); //position the brick
	model_matrix = scale(model_matrix, vec3(11.0f, 11.0f, 11.0f));
	updateMatrices();
	brick.draw();

	//Ground
	model_matrix = translate(mat4(1.0), vec3(0.0, -1.0, 0.0)); //position the ground
	model_matrix = rotate(model_matrix, -90.0f, vec3(1.0f, 0.0f, 0.0f)); //Make sure we rotate so that the normal is up!
	model_matrix = scale(model_matrix, vec3(11.0f, 11.0f, 11.0f));
	updateMatrices();
	ground.draw();
	
	//cube 1
	model_matrix = translate(mat4(1.0), vec3(4.0f, 0.1f, -4.0f));  //position the cube
	model_matrix = rotate(model_matrix, cube_rot_angle, vec3(0.0f, 1.0f, 0.0f));
	model_matrix = scale(model_matrix, vec3(0.5f, 0.5f, 0.5f));
	updateMatrices();
	if (drawCube1) //check to see if we should still draw it.
		cube.draw();
	//cube 2
	model_matrix = translate(mat4(1.0), vec3(-4.0f, 0.1f, -4.0f));  //position the cube
	model_matrix = rotate(model_matrix, cube_rot_angle, vec3(0.0f, 1.0f, 0.0f));
	model_matrix = scale(model_matrix, vec3(0.5f, 0.5f, 0.5f));
	updateMatrices();
	if (drawCube2) //check to see if we should still draw it.
		cube.draw();
	//cube 3
	model_matrix = translate(mat4(1.0), vec3(-4.0f, 0.1f, 4.0f));  //position the cube
	model_matrix = rotate(model_matrix, cube_rot_angle, vec3(0.0f, 1.0f, 0.0f));
	model_matrix = scale(model_matrix, vec3(0.5f, 0.5f, 0.5f));
	updateMatrices();
	if (drawCube3) //check to see if we should still draw it.
		cube.draw();
	//cube 4
	model_matrix = translate(mat4(1.0), vec3(4.0f, 0.1f, 4.0f));  //position the cube
	model_matrix = rotate(model_matrix, cube_rot_angle, vec3(0.0f, 1.0f, 0.0f));
	model_matrix = scale(model_matrix, vec3(0.5f, 0.5f, 0.5f));
	updateMatrices();
	if (drawCube4) //check to see if we should still draw it.
		cube.draw();

	//ground stripes 
	model_matrix = translate(mat4(1.0), vec3(0.0, -0.9, 0.0));  
	model_matrix = rotate(model_matrix, -90.0f, vec3(1.0f, 0.0f, 0.0f)); //Make sure we rotate so that the normal is up!
	model_matrix = scale(model_matrix, vec3(10.0f, 10.0f, 10.0f));
	updateMatrices();
	stripes.draw();
	 
	model_matrix = translate(mat4(1.0), vec3(0.0, 5.0, 0.0));
	model_matrix = scale(model_matrix, vec3(0.5f, 0.5f, 0.5f));
	updateMatrices();
	ball.draw();
	glFlush();
}
