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
	float mask[21]=float[](
	0.000003,	0.000023,	0.000152,	0.000806,	0.003426,	0.011654,	0.031728,	0.069139,	0.120596,
		0.168378,	0.188193,	0.168378,	0.120596,	0.069139,	0.031728,	0.011654,	0.003426,	0.000806,	0.000152,	0.000023,	0.000003
);
	for(int i=0;i<21;i++)
		outColor.xyz+=mask[i]*(texture(texFramebuffer, Texcoord+(i-10)*direction).xyz-vec3(darken));
	outColor.xyz=max(outColor.xyz, vec3(0));
	outColor.w=1;
}