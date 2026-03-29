#version 330 core

precision highp float;

// buffer inputs
layout(location = 0) in vec3 b_position;
layout(location = 1) in vec3 b_normal;
layout(location = 2) in vec2 b_texcoords;

struct Light {
	vec3 position;
	vec3 powerDensity;
};

// uniform variables
uniform Light light;
uniform vec3 camera;

uniform mat4 M;
uniform mat4 MI;
uniform mat4 V;
uniform mat4 P;
uniform mat4 L;

// pipeline outputs
out vec2 p_texcoords;
out vec3 p_normal_vec;
out vec3 p_position;
out vec3 p_light_dir;
out vec3 p_view_dir;
out vec4 p_light_space_pos;

void main() {
	vec4 w_position = vec4(b_position, 1) * M;
	gl_Position = w_position * V * P;
	p_position = vec3(w_position);
	p_light_space_pos = L * w_position;

	p_light_dir = normalize(light.position - p_position);
	p_view_dir = normalize(camera - p_position);
	p_normal_vec = normalize(vec3(MI * vec4(b_normal, 0.0)));

	p_texcoords = b_texcoords;
}