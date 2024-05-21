#version 430

//uniform vec2 CursorPosition;
uniform mat4 InterfaceProjectionMatrix;

layout(location = 0) in vec4 Vertex;	// <vec2 position, vec2 texture>

out vec2 TexCoords;

void main()
{
    gl_Position = InterfaceProjectionMatrix * vec4(Vertex.xy, 0.0, 1.0);
    TexCoords = Vertex.zw;
}  