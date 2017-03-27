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

layout(location = 3) in mat4 Transform;


void main()
{
    gl_Position = Transform * vec4(VertexPosition, 1.0);
}
