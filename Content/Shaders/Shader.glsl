#ifdef VERTEX_SHADER

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

uniform mat4 u_ViewProjection;

void main()
{
	v_TexCoords = a_TexCoords;
	gl_Position = u_ViewProjection * a_Position;
}

#endif  // VERTEX_SHADER

#ifdef FRAGMENT_SHADER

in vec2 v_TexCoords;

out vec4 FragColour;

uniform sampler2D u_Texture;

void main()
{
	FragColour = texture(u_Texture, v_TexCoords);
}

#endif // FRAGMENT_SHADER
