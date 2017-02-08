#version 410

#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
void main()
{
    float redAberration = .002;
    float greenAberration = 0.0;
    float blueAberration = -.002;

    vec3 aberration = vec3(redAberration, greenAberration, blueAberration);
	aberration*=abs(Texcoord.x-.5f);

    vec3 col;
    col.x = texture(texFramebuffer,vec2(Texcoord.x+aberration.x,Texcoord.y)).x;
    col.y = texture(texFramebuffer,vec2(Texcoord.x+aberration.y,Texcoord.y)).y;
    col.z = texture(texFramebuffer,vec2(Texcoord.x+aberration.z,Texcoord.y)).z;

	outColor=vec4(col, 1);
}