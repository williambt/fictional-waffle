#version 330 core

layout (location = 0) in vec4 vPos;

uniform mat4 modelMat;
uniform mat4 projectionMat;

void main()
{
	gl_Position = projectionMat * modelMat * vPos;
}