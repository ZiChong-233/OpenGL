#version 460 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aUV;
layout(location=2) in vec3 aNormal;

uniform float time;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat3 normalMatrix;

//aPosition作为attribute(顶点属性)传入，原则上不允许更改
void main()
{
/*
	float scale = 1.0 / time ;
	vec3 sPos = aPos * scale;
	gl_Position = vec4(sPos, 1.0);
*/
	vec4 transformPosition = vec4(aPos, 1.0);
	transformPosition = modelMatrix * transformPosition;
	worldPosition = transformPosition.xyz;
	gl_Position = projectionMatrix * viewMatrix * transformPosition;
	uv = aUV;
	//normal = transpose(inverse(mat3(modelMatrix))) * aNormal;
	normal = normalMatrix * aNormal;
}