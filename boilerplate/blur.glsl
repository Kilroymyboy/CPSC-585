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
	float mask[11]=float[](
0.0093,	0.028002,	0.065984,	0.121703,	0.175713,	0.198596,	0.175713,	0.121703,	0.065984,	0.028002,	0.0093
);
	for(int i=0;i<11;i++)
		outColor.xyz+=mask[i]*(texture(texFramebuffer, Texcoord+(i-5)*direction).xyz-vec3(darken));
	outColor.w=1;
}