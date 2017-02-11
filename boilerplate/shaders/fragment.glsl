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
    FragmentColour = vec4(1, 1, 1, 0)*dot(-LightDirection, Normal);
    if(FragmentColour.x<0)FragmentColour.x=0;
    if(FragmentColour.y<0)FragmentColour.y=0;
    if(FragmentColour.z<0)FragmentColour.z=0;
    FragmentColour.xyz+=AmbientLight;
	int hits=0;
	for(int i=-2;i<3;i++)for(int j=-1;j<2;j++)
		if(texture(shadowMap, ShadowCoord.xy+vec2(j/3201.1,i/2402.9)).x >= ShadowCoord.z -0.002)
			hits++;
	FragmentColour.xyz*=Color/15.0*hits;
	FragmentColour.xyz+=EmissionColor;
}
