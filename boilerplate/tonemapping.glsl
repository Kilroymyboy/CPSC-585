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
uniform sampler2D texFramebuffer;
void main()
{
	vec3 t=texture(texFramebuffer, Texcoord).xyz;
	float exposure=1.0, gamma=1.2;
	t=vec3(1)-exp(-t*exposure);
	t=pow(t, vec3(1/gamma));
	outColor=vec4(t, 1);
}