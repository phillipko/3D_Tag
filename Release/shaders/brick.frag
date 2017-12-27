//FileName:		brick.frag
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn

//This fragment shader implements the Phong Reflection Model and Examples 8.4, 8.12, and 8.20 in the OpenGL 
//Programming Guide (8th edition) to draw a brick pattern on a polygon using Procedural Texturing.   
//ACKNOWLEDGEMENT: I got some help from: http://www.gamedev.net/topic/658486-glsl-send-array-receive-struct/ on how 
//to pass the array of structs with the light info to the shader.

#version 420 core

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
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;

	uint isEnabled;
};

layout(binding = 0,std140) uniform Lights
{
	LightProperties lights[4];
	vec3 globalAmbientLight;
	int totalLights; //Number of lights in this application
};

layout(binding = 35,std140) uniform Matrices
{
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;
};

layout (binding = 0, r32ui) uniform uimage2D output_buffer;

in vec4 pos; //position of the fragment in "eye" coordinates
in vec4 fColor;
in vec3 normal; //orientation of the normal in "eye" coordinates
in float shininess;

//"New" variables for the brick procedural shader
in vec2 MCposition;
uniform vec3 BrickColor, MortarColor;
uniform vec2 BrickSize;
uniform vec2 BrickPct;
uniform sampler2D Noise;

out vec4 FragColor;

//Implements examples 8.4 and 8.12 in the the OpenGL Programming Guide (8th edition)
#define Integral(x, p, notp) ((floor(x)*(p)) + max(fract(x)-(notp), 0.0))
vec3 makeBrickTex()
{
	vec3 color;
	vec2 position, fw, useBrick;
	vec2 MortarPct = vec2(1.0 - BrickPct.x, 1.0 - BrickPct.y); 
	
	position = MCposition / BrickSize;
	if (fract (position.y * 0.5) > 0.5)
		position.x += 0.5;

	//Calculate the filter size then perform filtering to do antialiasing on the brick pattern
	fw = fwidth(position);	
	useBrick = (Integral(position + fw, BrickPct, MortarPct) - Integral(position, BrickPct, MortarPct)) / fw;

	//position = fract(position);
	//useBrick = step(position, BrickPct);
	
	color = mix (MortarColor, BrickColor, useBrick.x * useBrick.y); //k_dif in the Phong reflectance model
	return color;
}

//A variation of example 8.20 on page 478 in the the OpenGL Programming Guide (8th edition)
float addNoise()
{
	vec4 noisevec = texture(Noise, MCposition);
	float intensity = min(1.0, noisevec[3]);
	return intensity;
}


void main()
{
	//TODO #1: Implement the procedural shader for the brick in the makeBrickTex() function
	//by following Example 8.4 on page 421 of the OpenGL Programming Guide (8th edition)
	vec3 color = makeBrickTex();
	
	//TODO #2: Perform antialiasing on the brick texture by modifying the makeBrickTex() function as
	//described in Example 8.12 on pages 456 and 457 of the OpenGL Programming Guide (8th edition)

	//TODO #3: Add some noise to the color	
	color *= addNoise();

	vec3 surfaceDiffuseColor = color; //use the color computed by the procedural texture code as the diffuse color 
	vec3 surfaceSpecularColor = vec3(0.0, 0.0, 0.0); //Make it black so that there will be no specular highlights
	vec3 scatteredLight = globalAmbientLight*surfaceDiffuseColor; //initialize scatteredLight to the global ambience*fragment color
	vec3 reflectedLight = vec3(0.0, 0.0, 0.0);
	
	//first, loop over all of the lights
	for (int lightNum = 0; lightNum < totalLights; lightNum++)
	{
		if (lights[lightNum].isEnabled == 1)  //Make sure the light is on
		{
			vec3 I = lights[lightNum].color.rgb;  //Intensity (color) of the light
			vec3 L; //this will be the L vector in the Phong reflectance model (lightDirection in the code on pages 377 and 378)
			vec4 lightPos_eyeCoords = view_matrix*lights[lightNum].position; //put light position in "eye" coordinates
			vec3 n = normal;

			float f; //attenuation in the phong reflectance model
				
			if (lights[lightNum].position.w > 0.1)  //a point light
			{
				vec4 LTemp = lightPos_eyeCoords - pos;  //Calculate the L vector
				L = vec3(LTemp.x, LTemp.y, LTemp.z); //remove the w value from the L vector
				float d = length(L);  //d in the phong reflectance model (lightDistance in the code on pages 377 and 378)
				L = L / d; //normalize L

				//Calculate the attenuation factor, f in the phong reflectance model
				float attenuationDenominator =	lights[lightNum].constantAttenuation +
												lights[lightNum].linearAttenuation * d +
												lights[lightNum].quadraticAttenuation * d * d;
				if (attenuationDenominator == 0) attenuationDenominator = 1;  //make sure we don't divide by 0
				f = 1.0 / attenuationDenominator;

				if (lights[lightNum].spotLightValues.x > 0)  //This is a spotLight
				{
					vec4 spotConeDir_eyeCoords = view_matrix*lights[lightNum].spotConeDirection;  
					vec3 spotVector = -normalize(vec3(spotConeDir_eyeCoords.x, spotConeDir_eyeCoords.y, spotConeDir_eyeCoords.z));
					float spotCos = dot (L, spotVector);
					if (spotCos < lights[lightNum].spotLightValues.y)  //Spotlight should not be visible
					{
						f = 0; //negate this light's effects
					}
					else //spotlight is shining on this fragment
					{
						f *= pow (spotCos, lights[lightNum].spotLightValues.z);  //Remember, spotLightValues.z is the spotExponent
					}
				}
			}
			else //directional light
			{
				L = normalize(vec3(lightPos_eyeCoords.x, lightPos_eyeCoords.y, lightPos_eyeCoords.z)); //remove w value to form the L vector and normalize
				f = 1.0;  //no attenuation for directional lights
			}

			float diffuseModifier = max ( 0.0, dot(n, L) );
			float specularModifier = 0.0;
			vec4 viewerPos = vec4(0.0, 0.0, 0.0, 1.0); //The viewer is at (0,0,0) in eye space
			vec4 vTemp = viewerPos - pos;
			vec3 v = normalize(vec3(vTemp.x, vTemp.y, vTemp.z)); //calculate v, the vector towards the viewer
			if (diffuseModifier > 0.001) //if little or no diffuse lighting, then there should not be a specular 
			{							 //highlight since the light is not shining directly on the object	 
				vec3 r = normalize( reflect (-L, n) );  //find r by "reflecting" lightDirection (L) around n
				specularModifier = pow (max ( 0.0, dot(r, v)), shininess );
			}
			//Accumulate all the light's effects
			reflectedLight +=	f *													//attenuation
								( (I * surfaceDiffuseColor * diffuseModifier) +		//diffuse term
								 (I * surfaceSpecularColor * specularModifier) );	//specular term
		}
	}

	vec3 sumOfLights = scatteredLight + reflectedLight;
	vec3 rgb = min ( sumOfLights, vec3(1.0, 1.0, 1.0) );  //clamp lighting at all white
	FragColor = vec4(rgb.r, rgb.g, rgb.b, fColor.a);  //use the fragment's original alpha

	imageAtomicAdd(output_buffer, ivec2(gl_FragCoord.xy), 1); //See page 581 in the OpenGL Programming Guide (8th edition) 
}