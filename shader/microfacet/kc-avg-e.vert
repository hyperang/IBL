#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform int index;

out vec2 TexCoords;

void main()
{
    TexCoords = vec2(aTexCoords.x, index);
	gl_Position = vec4(aPos, 1.0);
}