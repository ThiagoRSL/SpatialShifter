#version 460

// In the geometry shader, we start by defining the kind of primitive that this geometry shader
// expects to receive. The first layout statement indicates that this geometry shader will receive
// point primitives.
layout( triangles ) in;

// The next layout statement indicates the kind of primitives produced by this geometry shader,
// and the maximum number of vertices that will be output.
// In this case, we want to produce a single quad for each point received, so we indicate that the
// output will be a triangle strip with a maximum of four vertices.
layout( triangle_strip, max_vertices = 3 ) out;

in vec4 original_position[];
in vec3 vColor[];
in vec2 teTexCoord[];

out vec3 gsColor;
out vec3 gsNormal;
out vec2 gsTexCoord;
out vec4 gsWorldPosition;

void main()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 normal = normalize(cross(b, a));

    for (int i = 0; i < 3; i++)
    {  
        gsColor = vColor[i];
        gsNormal = normal;
        gsTexCoord = teTexCoord[i];
        gsWorldPosition = original_position[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}