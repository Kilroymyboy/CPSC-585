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

uniform vec2 large;
uniform vec2 small;

void main()
{
	outColor=vec4(0);
	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			outColor+=texture(texFramebuffer, (Texcoord+vec2(i/1280.0/2, j/720.0/2))*2);
		}
	}
	outColor.xyz/=vec3(4.0);
	outColor.w=1;
	
	outColor=texture(texFramebuffer, Texcoord);
}