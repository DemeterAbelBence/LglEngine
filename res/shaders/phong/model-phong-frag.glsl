#version 330 core

precision highp float;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 powerDensity;
};

uniform sampler2D texture_base_color;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;

uniform Material material;
uniform Light light;
uniform int meshFrame;

uniform float w_amb;
uniform float w_dif;
uniform float w_spe;

in vec2 texcoords;
in vec3 normal_vec;
in mat3 TBN;
in vec3 position;
in vec3 light_dir;
in vec3 view_dir;

out vec4 frag_col;

void main() {
	if (meshFrame == 1) {
		frag_col = vec4(1.0, 1.0, 1.0, 0.1);
		return;
	}

	vec3 normal_vec = vec3(texture(texture_normal, texcoords));
	normal_vec = normalize(normal_vec * 2.0 - 1.0);
	normal_vec = normalize(normal_vec * TBN); 
	normal_vec.y *= -1;

	vec3 half_dir = normalize(light_dir + view_dir);
	float light_cos = max(dot(normal_vec, light_dir), 0.0);
	float view_cos = max(dot(normal_vec, half_dir), 0.0);

	vec3 ka = 0.159 * vec3(texture(texture_base_color, texcoords));
	vec3 kd = 0.786 * vec3(texture(texture_diffuse, texcoords));
	vec3 ks = 1.274 * vec3(texture(texture_specular, texcoords));

	float sh = 10.0;
	float dist = length(position - light.position);
	vec3 at = light.powerDensity / (pow(dist, 1.5));
	vec3 radiance = ka + at * kd * light_cos + at * ks * pow(view_cos, sh);
	
	frag_col = vec4(radiance, 1.0);
}