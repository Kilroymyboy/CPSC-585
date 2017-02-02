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
    float redAberration = 1;
    float greenAberration = 0;
    float blueAberration = -1;

    vec3 aberration = vec3(redAberration, greenAberration, blueAberration);
	aberration*=abs(Texcoord.x-640)/640;

    vec3 col;
    col.x = texelFetch(texFramebuffer,ivec2(Texcoord.x+aberration.x,Texcoord.y),0).x;
    col.y = texelFetch(texFramebuffer,ivec2(Texcoord.x+aberration.y,Texcoord.y),0).y;
    col.z = texelFetch(texFramebuffer,ivec2(Texcoord.x+aberration.z,Texcoord.y),0).z;
//	col.xyz = texelFetch(texFramebuffer,ivec2(Texcoord.x,Texcoord.y), 0).xyz;
	outColor=vec4(col, 1);
}