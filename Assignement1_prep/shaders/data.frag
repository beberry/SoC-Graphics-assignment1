// Minimal fragment shader

#version 400


in vec4 fcolour;

in vec3 fnormal, flightdir;
in vec4 fdiffusecolour, fambientcolour, fposition;
in float fattenuation;
in vec2 ftexcoord;

vec3 global_ambient = vec3(0.05, 0.05, 0.05);
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);

out vec4 outputColor;

uniform uint emitmode;
uniform sampler2D tex1;

in float u1;
in float u2;

void main()
{
	vec3 emissive = vec3(0);

	// Ambient lighting calculation;
	vec3 ambient = fcolour.xyz *0.2;

	// Diffuse lighting calculation
	vec3 diffuse = max(dot(fnormal, flightdir), 0.0) * fcolour.xyz;

	// Specular lighting caluclation
	vec3 V = normalize(-fposition.xyz);	
	vec3 R = reflect(-flightdir, fnormal);
	vec3 specular = pow(max(dot(R, V), 0.0), 8.0) * specular_albedo;

	if (emitmode == 1) emissive = vec3(1.0, 1.0, 0.8);

	
	//outputColor = vec4(fattenuation*(ambient+diffuse+specular)+global_ambient+emissive, 1.0);
	outputColor = vec4(fattenuation*(ambient+diffuse+specular)+global_ambient+emissive, 1.0);
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
	outputColor =  texcolour*outputColor;

	//outputColor = vec4(1.0,0.0,0.0,1.0);
}