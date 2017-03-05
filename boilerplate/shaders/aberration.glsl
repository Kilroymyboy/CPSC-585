#version 410

#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
void main()
{
	float dist=(Texcoord.x-0.5)*(Texcoord.x-0.5)+(Texcoord.y-0.5)*(Texcoord.y-0.5);
	float f=1+dist*(.25+.01*sqrt(dist));

	vec2 coord=(Texcoord-vec2(.5,.5))*f*.9+vec2(.5,.5);

    float redAberration = .003;
    float greenAberration = 0.0;
    float blueAberration = -.003;

    vec3 aberration = vec3(redAberration, greenAberration, blueAberration);
	aberration*=abs(Texcoord.x-.5f);

    vec3 col;
    col.x = texture(texFramebuffer,vec2(coord.x+aberration.x,coord.y)).x;
    col.y = texture(texFramebuffer,vec2(coord.x+aberration.y,coord.y)).y;
    col.z = texture(texFramebuffer,vec2(coord.x+aberration.z,coord.y)).z;

	outColor=vec4(col, 1);
}