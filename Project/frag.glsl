#version 330 core

in GS_OUT
{
	vec2 TexCoord;
	vec4 normalWorld;
	vec4 worldPosition;
} fs_in;

out vec4 color;

uniform sampler2D ourTexture;
uniform vec4 lightColor;
uniform vec4 lightPosition;

void main()
{
	float ambientStrength = 0.15f;
	vec4 ambient = ambientStrength * lightColor;

	color = ambient * texture(ourTexture, fs_in.TexCoord);

}