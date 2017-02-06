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
layout(location=0)out vec4 outColor;
uniform sampler2D texFramebuffer;
void main()
{
	const float gamma=2.2;
	vec4 t=texture(texFramebuffer, Texcoord);
	outColor=vec4(t.xyz/(t.xyz+vec3(1,1,1)), 1);
	outColor.xyz=pow(outColor.xyz, vec3(1.0/gamma));
}