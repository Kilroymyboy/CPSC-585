// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410


#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;

layout(location = 2) uniform mat4 Projection;
layout(location = 3) uniform mat4 ModelView;

layout(location = 4) uniform vec3 LightDirection;
layout(location = 5) uniform vec3 AmbientLight;

layout(location = 6) uniform vec3 Color;
layout(location = 7) uniform vec3 EmissionColor;


//layout(location = 5) uniform vec3 LightDirection;


// output to be interpolated between vertices and passed to the fragment stage
out vec3 Normal;

void main()
{
    gl_Position = Projection * ModelView * vec4(VertexPosition, 1.0);
	Normal=mat3(ModelView)*VertexNormal;
}
