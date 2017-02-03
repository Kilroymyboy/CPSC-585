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
	outColor=vec4(0,0,0,1);
	float mask[7]=float[](0.03,0.11,0.22,0.28,0.22,0.11,0.03);
	for(int i=0;i<7;i++){
			vec4 t=mask[i]*texture(texFramebuffer, Texcoord+vec2((i-3)/543.21,0));

			outColor.xyz+=t.xyz;
	}
	outColor.xyz=max(outColor.xyz-vec3(1,1,1), vec3(0,0,0));
}