#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

//in vec2 TexCoord;
//in vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in VS_OUT{
	//vec3 ourColor;
	vec2 TexCoord;
} gs_in[];

out GS_OUT{
	//vec3 ourColor;
	vec2 TexCoord;
	vec4 normalWorld;
	vec4 worldPosition;
} gs_out;




void main()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 normal = normalize(cross(a, b));
	gs_out.normalWorld = model * vec4(normal, 0.0f);

		for (int i = 0; i < 3; i++)
	{
		gl_Position = projection*view*model*gl_in[i].gl_Position;
		gs_out.TexCoord = gs_in[i].TexCoord;
		gs_out.worldPosition = model * gl_in[i].gl_Position;
		EmitVertex();
	}
		EndPrimitive();

	for (int i = 0; i < 3; i++)
	{
		gl_Position = projection*view*model*(gl_in[i].gl_Position + vec4(normal,0.0f)); //chane to minus to get it to rotate behind
		gs_out.TexCoord = gs_in[i].TexCoord;
		gs_out.worldPosition = model * (gl_in[i].gl_Position + vec4(normal, 0.0f));
		EmitVertex();
	}
	EndPrimitive();

}
