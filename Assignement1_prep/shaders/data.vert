#version 400

// Define the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 textcoord;

// Uniforms
uniform mat4 model, view, projection;
uniform mat3 normalmatrix;
uniform vec4 lightPos;
uniform uint emitmode, textureMode, specularMode;

// My out definitions
out VS_OUT
{
	vec3 N, L;
	vec4 colour, diffusecolour, ambientcolour, P, world_coord;
	float attenuation;
} vs_out;

out VS_OUT
{
	vec2 texcoord;
	float u1, u2;
} vs_texture;

void main()
{
	if(textureMode == 1)
	{
		// Used for texturing without vertex seams.
		// Taken from http://vcg.isti.cnr.it/~tarini/no-seams/jgt_tarini.pdf 
		vs_texture.u1 = fract(textcoord.x);
		vs_texture.u2 = fract(textcoord.x+0.5)-0.5;

		vs_texture.texcoord = textcoord.xy;
	}

	vec4 position_h     = vec4(position, 1.0);		// Convert the (x,y,z) position to homogeneous coords (x,y,z,w)
	vec4 diffuse_albedo = vec4(1.0, 1.0, 1.0, 1.0);	// This is the vertex colour, used to handle the colourmode change
	vec3 light_pos3		= lightPos.xyz;			

	// Define our vectors to calculate diffuse and specular lighting
	mat4 mv_matrix = view * model;				// Calculate the model-view transformation
	vec4 P = mv_matrix * position_h;			// Modify the vertex position (x, y, z, w) by the model-view transformation
	vec3 N = normalize(normalmatrix * normal);	// Modify the normals by the normal-matrix (i.e. to model-view (or eye) coordinates )
	vec3 L = light_pos3 - P.xyz;				// Vector from the light position to the vertex in eye coordinates.
	float distanceToLight = length(L);			// For attenuation
	L = normalize(L);							// Normalise our light vector

	// Calculate the attenuation factor;
	float attenuation_k = 2.0;
    float attenuation = 1.0 / (1.0 + attenuation_k * pow(distanceToLight, 2));

	// Set Out variables
	vs_out.N = N;
	vs_out.P = P;
	vs_out.L = L;

	vs_out.colour	   = diffuse_albedo;
	vs_out.attenuation = attenuation;
	vs_out.world_coord = model*position_h;

	gl_Position = (projection * view * model) * position_h;
}