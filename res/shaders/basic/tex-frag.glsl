#version 330 core

precision highp float;

uniform sampler2D sun_texture;
uniform int meshFrame;

in vec2 tex_coord;
out vec4 frag_col;

struct Light {
	vec3 position;
	vec3 powerDensity;
};

uniform Light light;

void main() {
	if (meshFrame == 1) {
		frag_col = vec4(1, 1, 1, 0.4);
		return;
	}
	vec3 texCol = vec3(texture(sun_texture, tex_coord));
	vec3 emitedColor = light.powerDensity / 1000.0; 
	vec3 finalColor = 0.5 * emitedColor  + 0.5 * texCol;

	frag_col = vec4(finalColor, 1);
}