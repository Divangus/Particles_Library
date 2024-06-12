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
			int totalColumns = int(1.0 / frameSize.x);
            int row = vInstanceFrame / totalColumns;
            int column = vInstanceFrame % totalColumns;

            // Adjust column calculation to fix inversion by reversing the column index
            vec2 frameOffset = vec2(totalColumns - column - 1, row) * frameSize;

            // Correct texture coordinates by flipping along the x-axis
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