#version 430

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

uniform float DestructionAnimationParam;
uniform bool Destroyed;
uniform float ZoomFactor;

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
        // Posição do vértice
        vec4 vertexPos = gl_in[i].gl_Position;
        
        // Normal da face
        vec3 normal = normalize(cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));

        if (Destroyed) 
        {
            // Ajustar a normal para apenas afetar o plano X-Y
            vec3 adjustedNormal = normalize(vec3(normal.x, normal.y, 0.0f));

            // Ajustar a animação de destruição levando em consideração o zoom
            float adjustedDestructionParam = DestructionAnimationParam / ZoomFactor;

            // Aplicando o deslocamento na posição da face
            vertexPos.xyz -= adjustedNormal * adjustedDestructionParam;
        }

        // Definir a posição do vértice
        gl_Position = vertexPos;
        
        // Passar os dados para o fragment shader
        gUv = vUv[i];
        gNormal = vNormal[i];
        gColor = vColor[i];

        EmitVertex();
    }
    EndPrimitive();
}
