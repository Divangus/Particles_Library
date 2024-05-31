#version 430

in vec2 vTexCoord;
in vec4 vInstanceColor;

uniform bool text;
uniform sampler2D uTexture;

layout(location = 0) out vec4 oColor;

void main()
{
	if(text == true){
		
		oColor = vInstanceColor * texture(uTexture, vTexCoord);

	}
	else{
		
		oColor = vInstanceColor;

	}
}