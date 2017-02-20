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

layout(location=0) uniform int orientation;

uniform sampler2D tex0;
uniform sampler2D tex1;


void main()
{
	if(orientation==0){
		if(Texcoord.x<0.5){
			outColor=texture(tex0, vec2(Texcoord.x*2, Texcoord.y));
		}else{
			outColor=texture(tex0, vec2((Texcoord.x-0.5)*2, Texcoord.y));
		}
	}else{
		if(Texcoord.y<0.5){
			outColor=texture(tex0, vec2(Texcoord.x, Texcoord.y*2));
		}else{
			outColor=texture(tex0, vec2(Texcoord.x, (Texcoord.y-0.5)*2));
		}
	}
}