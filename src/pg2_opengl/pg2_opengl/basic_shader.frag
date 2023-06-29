#version 460 core

layout ( location = 0 ) out vec4 FragColor;

// inputs
in vec3 position_ws;
in vec3 unified_normal_ws;
in vec2 m_tex_coords;
in vec3 view_from_ws;
in vec3 m_color;
flat in uint out_index_material;

// uniforms
//uniform float amb_int;
uniform vec3 light_position; 

vec3 tone_mapping(vec3 color, float gamma, float exposure){
	color *= exposure;
	color = color / ( color + vec3(1.0f) );
	color = pow(color, vec3(1.0f / gamma));
	return color;
}
void main( void ){	
	vec3 omega_o = normalize((view_from_ws - position_ws));
	vec3 omega_i = normalize(reflect( -omega_o, unified_normal_ws ));
	float cos_theta_o = dot(omega_o, unified_normal_ws);
	
	// diffuse element
	vec3 lightDir = normalize(light_position - position_ws);  
	float diff = max(dot(unified_normal_ws, lightDir), 0.0);

	// specular element
	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, unified_normal_ws);  
	float spec = pow(max(dot(omega_o, reflectDir), 0.0), 32);
	float specular = specularStrength * spec;  

	FragColor = vec4( (specular + diff) * m_color, 1.0f ); // amb_int
}