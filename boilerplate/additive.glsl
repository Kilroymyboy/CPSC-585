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
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;


void main()
{
	outColor=texture(tex0, Texcoord)+texture(tex1, Texcoord)+texture(tex2, Texcoord);
	outColor.w=1;
}