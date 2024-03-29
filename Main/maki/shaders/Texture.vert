#version 450 core
layout(location = 0) in vec3 point;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 model_matrx;

out vec4 vertex_color;
out vec2 vertex_uv;

void main()
{
	gl_Position = projection * model_matrx * vec4(point.x, point.y,point.z, 1.0);
	vertex_color = color;
	vertex_uv = uv;
}