#version 430

//uniform mat4 ScaleMatrix;
uniform mat4 RotationMatrix;
uniform mat4 MVP;
uniform vec3 CamTrans;
uniform float TimeNow;
uniform float MaxTTL;
uniform vec3 ColorDegradation;

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Velocity;
layout(location = 2) in vec4 Color;
layout(location = 3) in float TTL;

out float vLiving;
out vec4 vColor;

void main()
{
	gl_Position = MVP * vec4(Position.x - CamTrans.x, Position.y - CamTrans.y, Position.z, 1.0f); 
	vLiving = TTL / MaxTTL;

	float inverseLiving = (1.0 - vLiving);
	vColor = vec4(Color.x * vLiving + ColorDegradation.x * inverseLiving, 
					Color.y * vLiving + ColorDegradation.y * inverseLiving, 
					Color.z * vLiving  + ColorDegradation.z * inverseLiving, 1.0f);
}
