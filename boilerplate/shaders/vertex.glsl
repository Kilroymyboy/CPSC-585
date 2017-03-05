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
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in mat4 Transform;
// 3
// 4
// 5
layout(location = 6) uniform mat4 Projection;
layout(location = 7) uniform mat4 View;
//layout(location = 8) uniform mat4 Model;

layout(location = 9) uniform vec3 LightDirection;
layout(location = 10) uniform vec3 AmbientLight;

layout(location = 11) uniform vec3 Color;
layout(location = 12) uniform vec3 EmissionColor;

layout(location = 13) uniform mat4 shadowMVP;

out vec3 Normal;
out vec4 ShadowCoord;

void main()
{
    gl_Position = Projection * View * Transform * vec4(VertexPosition, 1.0);
	Normal=mat3(Transform)*VertexNormal;
	ShadowCoord = shadowMVP * Transform * vec4(VertexPosition, 1.0);
}
