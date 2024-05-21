#version 430

uniform vec2 CursorPosition;
uniform mat4 InterfaceProjection;

layout(location = 0) in vec3 Vertex;	// Vertex
layout(location = 1) in vec2 Texture;	// Colors

out vec2 vTex;

void main()
{
	//gl_Position = vec4(Vertex.x, Vertex.y, 1.0, 1.0);
	gl_Position = InterfaceProjection * vec4(Vertex.x + CursorPosition.x, Vertex.y - CursorPosition.y, 1.0, 1.0);
	gl_Position.z = 1.0f;

	vTex = vec2(Texture.x, Texture.y);
}
