// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410


#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) uniform mat4 MVP;


void main()
{
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
