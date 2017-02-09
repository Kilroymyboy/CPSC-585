// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

in vec2 Texcoord;
out vec4 outColor;

// large texture to downsample
uniform sampler2D texFramebuffer;

layout(location = 0) uniform vec2 direction;
layout(location = 1) uniform float darken;

void main()
{
	outColor=vec4(0);
	float mask[17]=float[](
0.008614,	0.015823,	0.026802,	0.041864,	0.060298,	0.080084,	0.098079,	0.110763,	0.115345,	0.110763,	0.098079,	0.080084,	0.060298,	0.041864,	0.026802,	0.015823,	0.008614
);
	for(int i=0;i<17;i++)
		outColor.xyz+=mask[i]*(texture(texFramebuffer, Texcoord+(i-8)*direction).xyz-vec3(darken));
	outColor.w=1;
}