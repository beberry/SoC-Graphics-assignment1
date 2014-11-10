// Minimal fragment shader

#version 400

in VS_OUT
{
	vec3 N, L;
	vec4 colour, diffusecolour, ambientcolour, P, world_coord;
	float attenuation;
} vs_out;

in vec2 ftexcoord;

vec3 global_ambient = vec3(0.07, 0.07, 0.07);
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);

out vec4 outputColor;

uniform uint emitmode, textureMode, specularMode, fogMode;
uniform sampler2D tex1;

in float u1;
in float u2;

vec4 fog_colour = vec4(0.1, 0.1, 0.1, 1.0);

vec4 fog(vec4 c)
{
	// Taken from "OpenGL SuperBible Sixth Edition", page 543
	float z = length(vs_out.P.xyz);

	float de = 0.025 * smoothstep(0.0, 6.0, 10.0 - vs_out.world_coord.y);
	float di = 0.045 * smoothstep(0.0, 40.0, 20.0  - vs_out.world_coord.y);

	float extinction = exp(-z * de);
	float inscattering = exp(-z * di);

	return c * extinction + fog_colour * (1.0 - inscattering);
}

vec4 fog_linear(vec4 c)
{
	// This example is taken from one of our lecture slides by Ian Martin.
	float fog_maxD = 17.0;
	float fog_minD = -2.0;

	float dist = length(vs_out.P.xyz);
	float fog_factor = (fog_maxD - dist) / (fog_maxD - fog_minD);
	fog_factor = clamp(fog_factor, 0.0, 1.0);

	return mix(fog_colour, c, fog_factor);
}

void main()
{
	vec3 emissive = vec3(0);

	// Ambient lighting calculation;
	vec3 ambient = vs_out.colour.xyz *0.3;

	// Diffuse lighting calculation
	vec3 diffuse = max(dot(vs_out.N, vs_out.L), 0.0) * vs_out.colour.xyz;

	// Specular lighting caluclation
	vec3 V = normalize(-vs_out.P.xyz);	
	vec3 R = reflect(-vs_out.L, vs_out.N);
	vec3 specular = vec3(0.0, 0.0, 0.0);
	
	if(specularMode == 1)
	{
		specular = pow(max(dot(R, V), 0.0), 8.0) * specular_albedo;
	}

	if (emitmode == 1) emissive = vec3(1.0, 1.0, 0.8);

	
	//outputColor = vec4(vs_out.attenuation*(ambient+diffuse+specular)+global_ambient+emissive, 1.0);
	outputColor = vec4(vs_out.attenuation*(ambient+diffuse+specular)+global_ambient+emissive, 1.0);

	if(textureMode == 1)
	{
		vec2 kk1, kk2, kk3;
		kk1.y = ftexcoord.y;
		kk2.y = ftexcoord.y;
		kk3.y = ftexcoord.y;

		kk1.x = u1;
		kk2.x = u2;


		float bb = fwidth(kk1);
		float aa = fwidth(kk2);

		if(bb <= aa)
		{
			kk3.x = kk1.x;
		}
		else
		{
			kk3.x = kk2.x;
		}

		vec4 texcolour = texture(tex1, kk3);
		outputColor = vec4(vs_out.attenuation*(texcolour.xyz*(ambient + diffuse+specular))+global_ambient+emissive, 1.0);
	}

	if(fogMode == 1)
	{
		// Linear fog
		outputColor = fog_linear(outputColor);
	}
	else if(fogMode == 2)
	{
		// Fog from the super bible
		outputColor = fog(outputColor);
	}

	//outputColor = vec4(1.0,0.0,0.0,1.0);
}