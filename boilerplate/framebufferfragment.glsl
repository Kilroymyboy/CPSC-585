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
    float redAberration = 32.0;
    float greenAberration = 0.0;
    float blueAberration = -32.0;

    vec3 aberration = vec3(redAberration / 1024, greenAberration / 1024, blueAberration / 1024);
	aberration*=abs(Texcoord.x-.5)*abs(Texcoord.y-.5);

    vec3 col;
    col.r = texture2D(texFramebuffer,vec2(Texcoord.x+aberration.x,Texcoord.y)).x;
    col.g = texture2D(texFramebuffer,vec2(Texcoord.x+aberration.y,Texcoord.y)).y;
    col.b = texture2D(texFramebuffer,vec2(Texcoord.x+aberration.z,Texcoord.y)).z;

	outColor=vec4(col, 1);
}