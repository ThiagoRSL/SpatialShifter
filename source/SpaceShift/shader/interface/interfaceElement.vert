#version 430

uniform vec2 Position;
uniform mat4 InterfaceProjection;

layout(location = 0) in vec2 VertexPosition;	// <vec2 position>
layout(location = 1) in vec2 VertexTexture;	    // <vec2 texture>

out vec2 TexCoords;

void main()
{
    gl_Position = InterfaceProjection * vec4(VertexPosition.x + Position.x, VertexPosition.y + Position.y, 0.0, 1.0);
    TexCoords = VertexTexture.xy;
}  