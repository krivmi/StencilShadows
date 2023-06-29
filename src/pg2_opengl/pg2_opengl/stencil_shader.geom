#version 460 core

layout ( triangles_adjacency ) in;
layout ( triangle_strip, max_vertices = 20 ) out; // 18
//layout ( line_strip, max_vertices = 200 ) out;

// uniform variables
uniform mat4 MVP; // (Model) View Projection matrix
uniform vec3 light_position; 

vec3 omega_i = vec3(0.0f,0.0f,0.0f);

out vec3 fColor;

void emitLine(vec3 point_A, vec3 point_B, bool inf_A, bool inf_B ){
	float mat_4_4_A = 1.0f;
	float mat_4_4_B = 1.0f;
	
	if(inf_A){
		mat_4_4_A = 0.0f;
	}
	if(inf_B){
		mat_4_4_B = 0.0f;
	}
	gl_Position = MVP * vec4(point_A, mat_4_4_A); 
	EmitVertex();
	gl_Position = MVP * vec4(point_B, mat_4_4_B);
	EmitVertex();
	EndPrimitive();
}

void main() {
	
	vec3 V0 = gl_in[0].gl_Position.xyz;
	vec3 V1 = gl_in[1].gl_Position.xyz;
	vec3 V2 = gl_in[2].gl_Position.xyz;
	vec3 V3 = gl_in[3].gl_Position.xyz;
	vec3 V4 = gl_in[4].gl_Position.xyz;
	vec3 V5 = gl_in[5].gl_Position.xyz;
	// CW
	vec3 N042 = normalize(cross( V4-V0, V2-V0 )); // centrální (face) normála
	vec3 N021 = normalize(cross( V2-V0, V1-V0 ));
	vec3 N243 = normalize(cross( V4-V2, V3-V2 ));
	vec3 N405 = normalize(cross( V0-V4, V5-V4 ));

	// CCW
	N042 = normalize(cross( V2-V0, V4-V0 )); 
	N021 = normalize(cross( V1-V0, V2-V0 ));
	N243 = normalize(cross( V3-V2, V4-V2 ));
	N405 = normalize(cross( V5-V4, V0-V4 ));

	vec3 V0_inf = V0  - light_position;
	vec3 V2_inf = V2  - light_position;
	vec3 V4_inf = V4  - light_position;

	omega_i = normalize(light_position - V0);

	vec3 offset =  normalize(V0 - light_position) * 0.01f;
	
	// Handle only light facing triangles 
	if ( dot( omega_i, N042 ) > 0 ) { // CCW
	
		// FRONT CAP
		fColor = vec3(0.0f,1.0f,0.0f);
		gl_Position = MVP * vec4(V0 + offset, 1.0f);
		EmitVertex();

		gl_Position = MVP * vec4(V4 + offset, 1.0f);
		EmitVertex();

		gl_Position = MVP * vec4(V2 + offset, 1.0f); 
		fColor = vec3(1.0f,1.0f,0.0f);
		EmitVertex();
		EndPrimitive();

		// BACK CAP - normála musí smìøovat dolù 
		fColor = vec3(0.0f,0.0f,1.0f);
		gl_Position = MVP * vec4(V0_inf, 0.0f);
		EmitVertex();

		gl_Position = MVP * vec4(V2_inf, 0.0f); 
		EmitVertex();

		gl_Position = MVP * vec4(V4_inf, 0.0f); 
		fColor = vec3(0.0f,1.0f,1.0f);
		EmitVertex();
		EndPrimitive();
	
	
		if ( sign( dot( omega_i, N042 ) ) != sign( dot( omega_i, N021 ) ) ) { // line is a silhouette
			fColor = vec3(1.0f,0.0f,0.0f);
			// Edge V0-V2
			/*
			gl_Position = MVP * vec4(V0, 1.0f);
			EmitVertex();

			gl_Position = MVP * vec4(V0_inf, 0.0f); 
			EmitVertex();

			gl_Position = MVP * vec4(V2, 1.0f); // for back cap
			EmitVertex();

			gl_Position = MVP * vec4(V2_inf, 0.0f); // for back cap
			fColor = vec3(1.0f,1.0f,0.0f);
			EmitVertex();
			EndPrimitive();*/
			gl_Position = MVP * vec4(V0 + offset, 1.0f);
			EmitVertex();

			gl_Position = MVP * vec4(V2 + offset, 1.0f); // for back cap
			EmitVertex();

			gl_Position = MVP * vec4(V0_inf, 0.0f); 
			EmitVertex();

			gl_Position = MVP * vec4(V2_inf, 0.0f); // for back cap
			fColor = vec3(1.0f,1.0f,0.0f);
			EmitVertex();
			EndPrimitive();
		}
		omega_i = normalize(light_position - V2);
		if ( sign( dot( omega_i, N042 ) ) != sign( dot( omega_i, N243 ) ) ) { // line is a silhouette
			fColor = vec3(1.0f,0.0f,0.0f);
			
			// Edge V2-V4
			/*
			gl_Position = MVP * vec4(V2, 1.0f);
			EmitVertex();

			gl_Position = MVP * vec4(V2_inf, 0.0f); // for back cap
			EmitVertex();

			gl_Position = MVP * vec4(V4, 1.0f); 
			EmitVertex();

			gl_Position = MVP * vec4(V4_inf, 0.0f); // for back cap
			fColor = vec3(1.0f,1.0f,0.0f);
			EmitVertex();
			EndPrimitive();*/
			gl_Position = MVP * vec4(V2 + offset, 1.0f);
			EmitVertex();

			gl_Position = MVP * vec4(V4 + offset, 1.0f); 
			EmitVertex();

			gl_Position = MVP * vec4(V2_inf, 0.0f); // for back cap
			EmitVertex();

			gl_Position = MVP * vec4(V4_inf, 0.0f); // for back cap
			fColor = vec3(1.0f,1.0f,0.0f);
			EmitVertex();
			EndPrimitive();
		}
	
		omega_i = normalize(light_position - V4);
		if ( sign( dot( omega_i, N042 ) ) != sign( dot( omega_i, N405 ) ) ) { // line is a silhouette
			fColor = vec3(1.0f,0.0f,0.0f);
		
			// Edge V0-V4
			/*
			gl_Position = MVP * vec4(V0, 1.0f);
			EmitVertex();

			gl_Position = MVP * vec4(V0_inf, 0.0f); // for back cap
			EmitVertex();

			gl_Position = MVP * vec4(V4, 1.0f); 
			EmitVertex();

			gl_Position = MVP * vec4(V4_inf, 0.0f); // for back cap
			fColor = vec3(1.0f,1.0f,0.0f);
			EmitVertex();
			EndPrimitive();*/
			gl_Position = MVP * vec4(V4 + offset, 1.0f); 
			EmitVertex();

			gl_Position = MVP * vec4(V0 + offset, 1.0f);
			EmitVertex();

			gl_Position = MVP * vec4(V4_inf, 0.0f); 
			EmitVertex();

			gl_Position = MVP * vec4(V0_inf, 0.0f); 
			fColor = vec3(1.0f,1.0f,0.0f);
			EmitVertex();			
			
			EndPrimitive();
		}
	}

	/*
	// LINES 
	omega_i = normalize(light_position - V0);
	if ( sign( dot( omega_i, N042 ) ) != sign( dot( omega_i, N021 ) ) ) {
		emitLine(V0, V2, false, false);
		emitLine(V0  - light_position, V2 - light_position, true, true);

		emitLine(V0  - light_position, V0, true, false);
		emitLine(V2  - light_position, V2, true, false);
	}

	omega_i = normalize(light_position - V2);
	if ( sign( dot( omega_i, N042 ) ) != sign( dot( omega_i, N243 ) ) ) {
		emitLine(V2, V4, false, false);
		emitLine(V2 -  light_position, V4 - light_position, true, true);

		emitLine(V4  - light_position, V4, true, false);
		emitLine(V2  - light_position, V2, true, false);
	}

	omega_i = normalize(light_position - V4);
	if ( sign( dot( omega_i, N042 ) ) != sign( dot( omega_i, N405 ) ) ) {
		emitLine(V4, V0, false, false);
		emitLine(V4 - light_position, V0 - light_position, true, true);

		emitLine(V0  - light_position, V0, true, false);
		emitLine(V4  - light_position, V4, true, false);
	}*/

	/*
	gl_Position = MVP * vec4(V0, 1.0f); // we should end up with the clip-space output position of the current vertex here
	EmitVertex();

	gl_Position = MVP * vec4(V2, 1.0f); // we should end up with the clip-space output position of the current vertex here
	EmitVertex();

	gl_Position = MVP * vec4(V4, 1.0f); 
	EmitVertex();
	EndPrimitive();
	
	
	// normals
	fColor = vec3(1.0f, 1.0f, 0.0f);
	gl_Position = MVP * vec4(V0, 1.0f); 
	EmitVertex();

	gl_Position = MVP * vec4(V0 - N042, 0.0f); 
	EmitVertex();

	gl_Position = MVP * vec4(V0 - N042, 1.0f); 
	fColor = vec3(0.0f, 1.0f, 1.0f);
	EmitVertex();
	EndPrimitive(); 
	*/
	//	gl_Position = MVP * ( vec4(av5, 1.0f) + vec4( 0.0f0f, 0.0f0f, 10.0f0f, 0.0f0f ) );



	fColor = vec3(1.0f, 1.0f, 1.0f);
}

