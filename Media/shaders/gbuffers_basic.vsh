#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color;
layout (location = 2) in vec2 texc;
layout (location = 3) in mat4 Model;

uniform mat4 View;
uniform mat4 Projection;

out vertex_in {
	vec4 texcolor;
	vec2 texcoord;
} Vertex;

void main() {
	vec4 v = vec4(position, 1.0);
	gl_Position = Projection * (View * (Model * v));
	Vertex.texcolor = vertex_color;
	Vertex.texcoord = texc;
}