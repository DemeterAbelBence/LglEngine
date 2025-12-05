#version 330 core

precision highp float;

in vec2 texcoords;
in vec3 normal_vec;
in mat3 TBN;
in vec3 position;
in vec3 light_dir;
in vec3 view_dir;

out vec4 frag_col;

struct Light {
	vec3 position;
	vec3 powerDensity;
};

uniform Light light;

uniform sampler2D texture_base_color;
uniform sampler2D texture_metalness;
uniform sampler2D texture_roughness;
uniform sampler2D texture_normal;
uniform sampler2D texture_ambient_occlusion;

//uniform float metallic;
uniform float roughness;
uniform float ao;

const float PI = 3.14159265359;
  
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

void main() {
    vec3 albedo = texture(texture_base_color, texcoords).xyz;
    float metallic = texture(texture_metalness, texcoords).x;
    float roughness = texture(texture_roughness, texcoords).x;
    float ao = 0.5;//texture(texture_ambient_occlusion, texcoords).x;

    vec3 N = texture(texture_normal, texcoords).xyz;
	N = normalize(N * 2.0 - 1.0);
	N = normalize(N * TBN); 
	//N.z *= -1;
    //N *= -1.0; // Invert the normal for correct lighting

    vec3 V = normalize(view_dir);
    vec3 F0 = vec3(0.24); 
    F0 = mix(F0, albedo, metallic);
	           
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 1; ++i) {
        vec3 L = normalize(light_dir);
        vec3 H = normalize(V + L);
        float dist = length(light.position - position);
        float attenuation = 1.0 / (dist * dist);
        vec3 radiance = light.powerDensity * attenuation;        
        
        float NDF = DistributionGGX(N, H, roughness);        
        float G = GeometrySmith(N, V, L, roughness);      
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  
        
        vec3 numerator = NDF * G * F;
        float denominator = max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;  
            
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }   
  
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    frag_col = vec4(color, 1.0);
}  