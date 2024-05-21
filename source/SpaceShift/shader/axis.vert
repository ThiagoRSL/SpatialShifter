
#version 430

uniform vec3 Position;
uniform mat4 RotationMatrix;
uniform mat4 MVP;

layout(location = 0) in vec3 Vertex;	// Vertex
layout(location = 1) in vec4 Color;	// Colors

out vec4 vPosition;
out vec4 vColor;

void main()
{
	gl_Position = MVP * ((vec4(Vertex.x, Vertex.y, Vertex.z, 1.0) + vec4(Position.xyz, 1.0)));
	vColor = Color;
}
