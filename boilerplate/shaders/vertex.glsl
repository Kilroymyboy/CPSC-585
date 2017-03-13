// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410


#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

// in
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;

// instanced
layout(location = 3) in mat4 Transform;
// 4
// 5
// 6
layout(location = 7) in vec3 InstanceColor;
layout(location = 8) in vec3 InstanceEmissionColor;

// uniforms
layout(location = 9) uniform mat4 Projection;
layout(location = 10) uniform mat4 View;

layout(location = 11) uniform vec3 LightDirection;
layout(location = 12) uniform vec3 AmbientLight;

layout(location = 13) uniform mat4 shadowMVP;
layout(location = 14) uniform int softShadow;

out vec3 Normal;
out vec2 TexCoord;
out vec4 ShadowCoord;
out vec3 Color;
out vec3 EmissionColor;

void main()
{
    gl_Position = Projection * View * Transform * vec4(VertexPosition, 1.0);
	Normal=mat3(Transform)*VertexNormal;
	TexCoord=VertexTexCoord;
	ShadowCoord = shadowMVP * Transform * vec4(VertexPosition, 1.0);
	Color=InstanceColor;
	EmissionColor=InstanceEmissionColor;
}
