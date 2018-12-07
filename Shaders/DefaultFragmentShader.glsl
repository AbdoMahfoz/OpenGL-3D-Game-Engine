#version 330 core

in vec3 color;
in vec2 uv;

out vec4 OutColor;

uniform sampler2D Sampler;

void main()
{
	OutColor = vec4(color, 1) * texture(Sampler, uv);
}