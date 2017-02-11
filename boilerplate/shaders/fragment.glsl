// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

// interpolated colour received from vertex stage
in vec3 Normal;
in vec4 ShadowCoord;

layout(location = 3) uniform mat4 View;

layout(location = 5) uniform vec3 LightDirection;
layout(location = 6) uniform vec3 AmbientLight;

layout(location = 7) uniform vec3 Color;
layout(location = 8) uniform vec3 EmissionColor;

layout(location = 9) uniform mat4 shadowMVP;

uniform sampler2D shadowMap;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void)
{
	float cosTheta = dot(-LightDirection, Normal);
    FragmentColour = vec4(1, 1, 1, 0)*cosTheta;
	FragmentColour.xyz=max(FragmentColour.xyz, vec3(0));
    FragmentColour.xyz+=AmbientLight;

	int hits=0;
	float bias = 0.002*tan(acos(cosTheta));
	bias = max(0, min(0.004, bias));

	if(ShadowCoord.x<0||ShadowCoord.x>1||ShadowCoord.y<0||ShadowCoord.y>1)
		hits=15;
	else for(int i=-2;i<3;i++)for(int j=-1;j<2;j++)
		if(texture(shadowMap, ShadowCoord.xy+vec2(j/3201.1,i/2402.9)).x >= ShadowCoord.z - bias)
			hits++;
	FragmentColour.xyz*=Color/15.0*hits;
	FragmentColour.xyz+=EmissionColor;
}
