#version 430

in vec2 vTexCoord;

uniform bool text;
uniform sampler2D uTexture;
uniform vec4 printColor;

layout(location = 0) out vec4 oColor;

void main()
{
	if(text == true){
		
		oColor = printColor * texture(uTexture, vTexCoord);

	}
	else{
		
		oColor = printColor;

	}
}