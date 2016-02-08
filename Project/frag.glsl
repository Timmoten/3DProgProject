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
	vec4 lightDirection = (lightPosition - fs_in.worldPosition);
	lightDirection = normalize(lightDirection);
	float angle = max(dot(fs_in.normalWorld, lightDirection),0.0f);
	vec4 diffuse = angle * lightColor;
	/*if (diffuse.x > 1)
		diffuse.x = 1;
	if (diffuse.y > 1)
		diffuse.y = 1;
	if (diffuse.z > 1)
		diffuse.z = 1;*/

	color = diffuse * texture(ourTexture, fs_in.TexCoord);

	//if (color.rgb == vec3(0.0, 0.0, 0.0)) //removes the black areas around the logo
		//discard;
}