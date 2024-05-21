#version 430

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

uniform float DestructionAnimationParam;
uniform bool Destroyed; 

in vec2 vUv[];
in vec3 vNormal[];
in vec4 vColor[];

out vec2 gUv; 
out vec3 gNormal; 
out vec4 gColor; 

void main() 
{
    for(int i = 0; i < 3; i++)
    {
        // Calcular a normal e usar para deslocar o vértice
        vec3 normal = normalize(cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));

        if (Destroyed) 
        {
            gl_Position = gl_in[i].gl_Position - vec4(normal * DestructionAnimationParam, 0.0);
        } 
        else 
        {
            gl_Position = gl_in[i].gl_Position;
        }
        EmitVertex();
    }
    EndPrimitive();
}