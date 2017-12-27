//FileName:		phong.frag
//Programmer:	Phillip Ko
//Date:			11/14/2014 
//The following code was adapted from Comp153 Lab 12 Code written by Dr. Cliburn


//This fragment shader is based on Example 7.8 on pages 377 and 378 of the OpenGL Programming Guide (8th edition)
//with some tweaks to support shininess as a vertex property and to allow the eye position to move around the scene.
//I also modified the code somewhat to make it closer to the Phong Reflectance Model that we discussed in class.
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

// The overdraw count buffer
layout (binding = 0, r32ui) uniform uimage2D output_buffer;

in vec4 position; //position of the fragment in "eye" coordinates
in vec4 color;
in vec3 normal; //orientation of the normal in "eye" coordinates
in float shininess;

out vec4 FragColor;

void main()
{
	//Implement the Phong reflectance model
	//initialize scatteredLight to the global ambience * fragment color
	vec3 scatteredLight = globalAmbientLight*color.rgb;
	vec3 reflectedLight = vec3(0.0, 0.0, 0.0);
	vec3 surfaceSpecularColor = vec3(1.0, 1.0, 1.0);//normally we want the specular color of the surface to be white
												//so that the color of the specular highlight is the color of the light	
	//first, loop over all of the lights
	for (int lightNum = 0; lightNum < totalLights; lightNum++)
	{
		if (lights[lightNum].isEnabled == 1)  //Make sure the light is on
		{
			vec3 I = lights[lightNum].color.rgb;  //Intensity (color) of the light
			vec3 L; //this will be the L vector in the Phong reflectance model (lightDirection in the code on pages 377 and 378)
			vec4 lightPos_eyeCoords = view_matrix*lights[lightNum].position; //put light position in "eye" coordinates
			
			vec3 surfaceDiffuseColor = color.rgb; //k_dif in the Phong reflectance model
			float f; //attenuation in the phong reflectance model
				
			if (lights[lightNum].position.w > 0.1)  //a point light
			{
				vec4 LTemp = lightPos_eyeCoords - position;  //Calculate the L vector
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

			float diffuseModifier = max ( 0.0, dot(normal, L) );
			float specularModifier;
			if (diffuseModifier > 0.001) //if little or no diffuse lighting, then there should not be a specular 
			{							 //highlight since the light is not shining directly on the object
				//Find the reflectance vector, r,  and the vector towards the viewer, v, instead of the half vector
				vec3 r = normalize( reflect (-L, normal) );  //find r by "reflecting" L about n
				vec4 viewerPos = vec4(0.0, 0.0, 0.0, 1.0); //The viewer is at (0,0,0) in eye space
				vec4 vTemp = viewerPos - position;
				vec3 v = normalize(vec3(vTemp.x, vTemp.y, vTemp.z)); //calculate v, the vector towards the viewer 
				specularModifier = pow (max ( 0.0, dot(r, v)), shininess );
			}
			else
			{
				specularModifier = 0.0;
			}

			//Accumulate all the light's effects
			reflectedLight +=	f *													//attenuation
								( (I * surfaceDiffuseColor * diffuseModifier) +		//diffuse term
								  (I * surfaceSpecularColor * specularModifier) );	//specular term
		}
	}
	vec3 sumOfLights = scatteredLight + reflectedLight;
	vec3 rgb = min ( sumOfLights, vec3(1.0, 1.0, 1.0) );  //clamp lighting at all white
	FragColor = vec4(rgb.r, rgb.g, rgb.b, color.a);  //use the fragment's original alpha

	imageAtomicAdd(output_buffer, ivec2(gl_FragCoord.xy), 1); //See page 581 in the OpenGL Programming Guide (8th edition)  	
}