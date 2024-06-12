#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in mat4 instanceModelMatrix;
layout(location = 6) in vec4 instanceColor;
layout(location = 7) in int instanceFrame;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec2 vTexCoord;
out vec4 vInstanceColor;
flat out int vInstanceFrame;

void main()
{
	vTexCoord = aTexCoord;
	vInstanceColor = instanceColor;
	vInstanceFrame = instanceFrame;
	gl_Position = projectionMatrix * viewMatrix * instanceModelMatrix * vec4(aPosition, 1.0);
}