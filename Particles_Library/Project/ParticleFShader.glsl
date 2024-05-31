#version 430

in vec2 vTexCoord;
in vec4 vInstanceColor;
flat in int vInstanceFrame;

uniform bool text;
uniform bool animatedText;
uniform sampler2D uTexture;
uniform vec2 frameSize;

layout(location = 0) out vec4 oColor;

void main()
{

	if(text == true){

		if(animatedText == true)
		{
			vec2 frameOffset = vec2(vInstanceFrame % int(1.0 / frameSize.x), vInstanceFrame / int(1.0 / frameSize.y)) * frameSize;
			vec2 texCoord = vTexCoord * frameSize + frameOffset;

			oColor = vInstanceColor * texture(uTexture, texCoord);
		}
		else{		
			oColor = vInstanceColor * texture(uTexture, vTexCoord);
		}		

	}
	else{
		
		oColor = vInstanceColor;

	}
}