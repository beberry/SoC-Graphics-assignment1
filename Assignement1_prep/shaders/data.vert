// Specify minimum OpenGL version
#version 400

// Define the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;

// My out definitions
out vec3 fnormal, flightdir;
out vec4 fcolour, fdiffusecolour, fambientcolour, fposition;
out float fattenuation;

// Uniforms
uniform mat4 model, view, projection;
uniform mat3 normalmatrix;
uniform vec4 lightPos;
uniform uint emitmode, colourmode;


void main()
{
	vec3 emissive = vec3(0);				// Create a vec3(0, 0, 0) for our emmissive light
	vec4 position_h = vec4(position, 1.0);	// Convert the (x,y,z) position to homogeneous coords (x,y,z,w)
	vec4 diffuse_albedo;					// This is the vertex colour, used to handle the colourmode change
	vec3 light_pos3 = lightPos.xyz;			

	// Switch the vertex colour based on the colourmode
	if (colourmode == 1)
		diffuse_albedo = colour;
	else
		diffuse_albedo = vec4(1.0, 0, 0, 1.0);

	//vec3 ambient = diffuse_albedo.xyz *0.2;



	// Define our vectors to calculate diffuse and specular lighting
	mat4 mv_matrix = view * model;		// Calculate the model-view transformation
	vec4 P = mv_matrix * position_h;	// Modify the vertex position (x, y, z, w) by the model-view transformation
	vec3 N = normalize(normalmatrix * normal);		// Modify the normals by the normal-matrix (i.e. to model-view (or eye) coordinates )
	vec3 L = light_pos3 - P.xyz;		// Calculate the vector from the light position to the vertex in eye space
	float distanceToLight = length(L);	// For attenuation
	L = normalize(L);					// Normalise our light vector


	// Pass to fragment shader.
	fnormal   = N;
	fposition = P;
	flightdir = L;



	// Calculate the attenuation factor;
	float attenuation_k = 2.0;
    float attenuation = 1.0 / (1.0 + attenuation_k * pow(distanceToLight, 2));

	// Set Out variables
	fcolour = diffuse_albedo;
	fattenuation = attenuation;
	
	gl_Position = (projection * view * model) * position_h;
}


