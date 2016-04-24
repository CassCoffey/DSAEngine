#version 430

in vec2 fragUV;
uniform sampler2D myTexture;

void main()
{
	gl_FragColor = texture(myTexture, fragUV);
}