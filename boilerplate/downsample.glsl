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

layout(location = 1) uniform vec2 large;
layout(location = 2) uniform int samples;

void main()
{
	outColor=vec4(0);
	for(int i=0;i<samples;i++){
		for(int j=0;j<samples;j++){
			outColor+=texture(texFramebuffer, (Texcoord+vec2(i/large.x, j/large.y)));
		}
	}
	outColor.xyz/=vec3(samples*samples);
	outColor.w=1;
}