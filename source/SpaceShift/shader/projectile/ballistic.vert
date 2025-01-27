#version 430

uniform vec3 Position;
uniform mat4 RotationMatrix;
uniform mat4 ScaleMatrix;
uniform mat4 MVP;
uniform float ZoomFactor;

layout(location = 0) in vec3 Vertex;	// Vertex
layout(location = 1) in vec2 Uv;		// UV
layout(location = 2) in vec3 Normal;	// Normal
layout(location = 3) in vec4 Color;		// Color

out vec2 vUv;
out vec3 vNormal;
out vec4 vColor;

void main()
{
	gl_Position = MVP * (RotationMatrix * (ScaleMatrix * vec4(Vertex.x, Vertex.y, Vertex.z, 1.0)) + vec4(Position.xyz, 0.0));
	//gl_Position = MVP * (vec4(Vertex.x, Vertex.y, Vertex.z, 1.0) + vec4(Position.xyz, 1.0));
	vUv = Uv;
    vNormal = Normal;
    vColor = Color;
}
