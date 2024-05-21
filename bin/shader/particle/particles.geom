#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float ParticleSize;

in float vLiving[];
in vec4 vColor[];
out float gLiving;
out vec4 gColor;
out vec2 TexCoords;

void main() 
{
    
	gLiving = vLiving[0];
	gColor = vColor[0];

    vec2 uv[4] = vec2[]
    (
        vec2(0, 0),
        vec2(1, 0),
        vec2(0, 1),
        vec2(1, 1)
    );

    for(int i = 0; i < 4; i++)
    {
        // Calcular a posição do vértice do quad
        vec4 position = gl_in[0].gl_Position;
        position.x += ParticleSize * (i == 1 || i == 3 ? 1 : -1);
        position.y += ParticleSize * (i > 1 ? 1 : -1);


        gl_Position = position;
        TexCoords = uv[i];

        
        EmitVertex();
    }
    EndPrimitive();
}

