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
//	outColor=vec4(texture(texFramebuffer, Texcoord).xyz-vec3(0,0,1), 1);
//	outColor.x=Texcoord.x;
	outColor=vec4(0,0,0,1);
	float mask[7]=float[](0.03,0.11,0.22,0.28,0.22,0.11,0.03);
	for(int i=0;i<7;i++){
		for(int j=0;j<7;j++){
			vec4 t=mask[i]*mask[j]*texture(texFramebuffer, Texcoord+vec2((i-3)/1280.0,(j-3)/720.0));
		//	t.xyz-=vec3(1,1,1);
			t.xyz=max(t.xyz, vec3(0,0,0));
			outColor.xyz+=t.xyz;
		}
	}
}