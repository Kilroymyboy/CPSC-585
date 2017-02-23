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
layout(location = 2) uniform float sigma;
layout(location = 3) uniform int size;

float g(float x){
	float c=2*sigma*sigma;
	return 1/sqrt(3.14159265*c)*pow(2.71828182, -(x*x)/c);
}

void main()
{
	outColor=vec4(0);

	for(int i=-size;i<size+1;i++)
		outColor.xyz+=g(i)*(texture(texFramebuffer, Texcoord+(i)*direction).xyz);
	outColor.xyz=max(outColor.xyz-vec3(darken), vec3(0));
	outColor.w=1;
}