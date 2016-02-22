#version 330 core

in GS_OUT
{
	vec2 TexCoord;
	vec4 normalWorld;
	vec4 worldPosition;
} fs_in;

uniform sampler2D ourTexture;

out vec4 color;


void main()
{
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	color = texture(ourTexture, fs_in.TexCoord);
}