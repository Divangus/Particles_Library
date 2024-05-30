#ifdef TEXTURED_GEOMETRY

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location = 0) in vec3 aPosition;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix

void main()
{
	gl_Position = projectionMatrix * modelViewMatrix * vec4(aPosition, 1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

layout(location = 0) out vec4 oColor;

void main()
{
	oColor = vec4(1.0);
}

#endif
#endif