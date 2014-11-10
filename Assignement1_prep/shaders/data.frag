// Minimal fragment shader

#version 410
layout(location = 0) in vec3 position;

in vec4 fcolour;

in vec3 fnormal, flightdir;
in vec4 fdiffusecolour, fambientcolour, fposition;
in float fattenuation;
in vec2 ftexcoord;


vec3 global_ambient = vec3(0.07, 0.07, 0.07);
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);

out vec4 outputColor;

uniform uint emitmode, textureMode, specularMode, fogMode;
uniform sampler2D tex1;

in float u1;
in float u2;

vec4 fog(vec4 c)
{
	//http://www.ozone3d.net/tutorials/glsl_fog/p03.php
	vec4 fog_colour = vec4(0.1, 0.1, 0.1, 1.0);
	float z = length(fposition.xyz);

	float de = 0.025 * smoothstep(0.0, 6.0, 10.0 - position.y);
	float di = 0.045 * smoothstep(0.0, 40.0, 20.0  - position.y);

	float extinction = exp(-z * de);
	float inscattering = exp(-z * di);

	return c * extinction + fog_colour * (1.0 - inscattering);
}

vec4 fog_linear(vec4 c)
{
	float fog_maxD = 17.0;
	float fog_minD = -2.0;
	vec4 fog_colour = vec4(0.1, 0.1, 0.1, 1.0);

	float dist = length(fposition.xyz);
	float fog_factor = (fog_maxD - dist) / (fog_maxD - fog_minD);
	fog_factor = clamp(fog_factor, 0.0, 1.0);

	return mix(fog_colour, c, fog_factor);
}

void main()
{
	vec3 emissive = vec3(0);

	// Ambient lighting calculation;
	vec3 ambient = fcolour.xyz *0.3;

	// Diffuse lighting calculation
	vec3 diffuse = max(dot(fnormal, flightdir), 0.0) * fcolour.xyz;

	// Specular lighting caluclation
	vec3 V = normalize(-fposition.xyz);	
	vec3 R = reflect(-flightdir, fnormal);
	vec3 specular = vec3(0.0, 0.0, 0.0);
	
	if(specularMode == 1)
	{
		specular = pow(max(dot(R, V), 0.0), 8.0) * specular_albedo;
	}

	if (emitmode == 1) emissive = vec3(1.0, 1.0, 0.8);

	
	//outputColor = vec4(fattenuation*(ambient+diffuse+specular)+global_ambient+emissive, 1.0);
	outputColor = vec4(fattenuation*(ambient+diffuse+specular)+global_ambient+emissive, 1.0);

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
		outputColor = vec4(fattenuation*(texcolour.xyz*(ambient + diffuse+specular))+global_ambient+emissive, 1.0);
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