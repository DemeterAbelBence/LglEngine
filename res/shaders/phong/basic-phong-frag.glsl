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

// uniform variables
uniform sampler2D texture_base_color;
uniform sampler2D DEPTH_MAP;

uniform Material material;
uniform Light light;
uniform int meshFrame;

// pipeline inputs
in vec2 p_texcoords;
in vec3 p_normal_vec;
in vec3 p_position;
in vec3 p_light_dir;
in vec3 p_view_dir;
in vec4 p_light_space_pos;

out vec4 frag_col;

/*float shadowCalculation(vec4 fragPosLightSpace, vec3 normal) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;

	float bias = 0.000001; 
	//max(0.0001 * (1.0 - dot(normal, normalize(light.position - p_position))), 0.0001);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(DEPTH_MAP, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(DEPTH_MAP, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
    return 0.2 * shadow;
}  */

void main() {
	if (meshFrame == 1) {
		frag_col = vec4(1.0, 1.0, 1.0, 0.1);
		return;
	}

	vec3 half_dir = normalize(p_light_dir + p_view_dir);
	float light_cos = max(dot(p_normal_vec, p_light_dir), 0.0);
	float view_cos = max(dot(p_normal_vec, half_dir), 0.0);

	vec3 ka = 0.5 * vec3(texture(texture_base_color, p_texcoords));
	vec3 kd = material.diffuse;
	vec3 ks = material.specular;
	float sh = material.shininess;
	float dist = length(p_position - light.position);
	vec3 at = light.powerDensity / (pow(dist, 1.5));

	vec3 radiance = ka + at * kd * light_cos + at * ks * pow(view_cos, sh);
	
	//float s = shadowCalculation(p_light_space_pos, p_normal_vec);
	//radiance -= vec3(s, s, s);

	//float d = texture(DEPTH_MAP, p_texcoords).r;

	frag_col = vec4(radiance, 0.5);
}