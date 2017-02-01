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
    float redAberration = .08;
    float greenAberration = 0.0;
    float blueAberration = -.08;

    vec3 aberration = vec3(redAberration, greenAberration, blueAberration);
	aberration*=abs(Texcoord.x-.5f)*abs(Texcoord.y-.5f);

    vec3 col;
    col.x = texture2D(texFramebuffer,vec2(Texcoord.x+aberration.x,Texcoord.y+100)).x;
    col.y = texture2D(texFramebuffer,vec2(Texcoord.x+aberration.y,Texcoord.y+100)).y;
    col.z = texture2D(texFramebuffer,vec2(Texcoord.x+aberration.z,Texcoord.y+100)).z;

	outColor=vec4(col, 1);
}