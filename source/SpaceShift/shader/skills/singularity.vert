#version 430

//uniform mat4 ScaleMatrix;
uniform vec3 Position;
uniform mat4 MVP;
uniform float Time;

layout(location = 0) in vec3 Vertex;	// Vertex
layout(location = 1) in vec4 Color;		// Color

out vec4 vColor;

void main()
{
    float rad = radians((Time / 5.0f) * 360.0f);
    float oscilacao = (sin(rad) + 1.0f) / 2.0f; // Isso vai oscilar entre 0 e 1

    vec4 corOscilante = Color + vec4(oscilacao * 0.1); // Adiciona a oscilação à cor base
    corOscilante = clamp(corOscilante, 0.0, 1.0); // Garante que a cor está no intervalo válido

    vColor = corOscilante;

	gl_Position = MVP * (vec4(Vertex.x, Vertex.y, Vertex.z, 1.0) + vec4(Position.xyz, 1.0));
}
