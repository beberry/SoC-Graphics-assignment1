/**
	A primitive graphics object - sphere.
	Some code taken from lab4 example. The code was extracted into its own
	class and new functionality was added.

	It is possible to define whether the sphere needs to be full / half-sphere.
	It is possible to define the radiuss of the sphere object.

	@author Ian Martin, Modified by Jekabs Stikanss
	@version 1.0, 03/11/2014
*/
#include "Sphere.h"

/* The original code for this object has been taken from Ian Martins lab4 example, modified by Jekabs Stikans */
Sphere::Sphere(GLfloat maxWidth, GLfloat maxHeight, bool halfSphere, GLuint textureID)
{
	this->maxWidth = maxWidth;
	this->maxHeight = maxHeight;
	this->drawmode = 3;
	this->halfSphere = halfSphere;

	this->textureID = textureID;

	try
	{
		/* Not actually needed if using one texture at a time */
		glActiveTexture(GL_TEXTURE0);

		/* load an image file directly as a new OpenGL texture */
		this->texID = SOIL_load_OGL_texture("textures/copper_3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if (this->texID == 0)
		{
			printf("TexID SOIL loading error: '%s'\n", SOIL_last_result());
		}
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}

	/* Define the texture behaviour parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Can't get this to work
	glGenerateMipmap(GL_TEXTURE_2D);
}


Sphere::~Sphere()
{
}

/* Make a sphere from two triangle fans (one at each pole) and triangle strips along latitudes */
GLuint Sphere::makeVBO(GLfloat numlats, GLfloat numlongs)
{
	GLuint i, j;
	this->numlats = numlats;
	this->numlongs = numlongs;

	/* Calculate the number of vertices required in sphere */
	this->vertexCount = 2 + ((numlats - 1) * numlongs);
	this->vertexPositions = new GLfloat[this->vertexCount * 3];
	this->vertexColours = new GLfloat[this->vertexCount * 4];
	makeUnitObject(numlats, numlongs);

	/* Define colours as the x,y,z components of the sphere vertices */
	for (i = 0; i < this->vertexCount; i++)
	{
		this->vertexColours[i * 4] = this->vertexPositions[i * 3];
		this->vertexColours[i * 4 + 1] = this->vertexPositions[i * 3 + 1];
		this->vertexColours[i * 4 + 2] = this->vertexPositions[i * 3 + 2];
		this->vertexColours[i * 4 + 3] = 1.f;
	}

	/* Generate the vertex buffer object */
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->vertexCount * 3, this->vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* this->vertexCount * 3, this->vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Generate texture coordinats */
	std::vector<GLfloat> texcoords1;

	GLfloat latstep = 1.0 / numlats;
	GLfloat longstep = 1.0 / numlongs;

	texcoords1.push_back(0.0);
	texcoords1.push_back(0.0);

	int k = 1;
	for (int i = 0; i < numlats-1; i++)
	{
		for (int x = 0; x < numlongs; x++)
		{
			texcoords1.push_back(x*longstep);
			texcoords1.push_back(i*latstep);

			k++;
			
		}
	}

	texcoords1.push_back(1.0);
	texcoords1.push_back(1.0);

	/* Generate the texture coordinate buffer */
	glGenBuffers(1, &this->textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, texcoords1.size()*sizeof(GLfloat), texcoords1.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/* Store the colours in a buffer object */
	/*glGenBuffers(1, &sphereColours);
	glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	/* Calculate the number of indices in our index array and allocate memory for it */
	GLuint numindices = ((numlongs * 2) + 2) * (numlats - 1) + ((numlongs + 2) * 2);
	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index

	// Define indices for the first triangle fan for one pole
	for (i = 0; i < numlongs + 1; i++)
	{
		pindices[index++] = i;
	}
	pindices[index++] = 1;	// Join last triangle in the triangle fan

	GLuint start = 1;		// Start index for each latitude row
	for (j = 0; j < numlats - 2; j++)
	{
		for (i = 0; i < numlongs; i++)
		{
			pindices[index++] = start + i;
			pindices[index++] = start + i + numlongs;
		}
		pindices[index++] = start; // close the triangle strip loop by going back to the first vertex in the loop
		pindices[index++] = start + numlongs; // close the triangle strip loop by going back to the first vertex in the loop

		start += numlongs;
	}

	// Define indices for the last triangle fan for the south pole region
	for (i = this->vertexCount - 1; i > (this->vertexCount - numlongs - 2); i--)
	{
		pindices[index++] = i;
	}

	pindices[index] = this->vertexCount - 2;	// Tie up last triangle in fan

	// Generate a buffer for the indices
	glGenBuffers(1, &this->elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete pindices;

	return this->vertexCount;
}

/************************************************************/
/* Functions define primitive objects						*/
void Sphere::makeUnitObject(GLuint numlats, GLuint numlongs)
{
	GLfloat DEG_TO_RADIANS = glm::pi<float>() / 180.f;
	GLuint vnum = 0;
	GLfloat x, y, z, lat_radians, lon_radians;

	/* Define north pole */
	this->vertexPositions[0] = 0; 
	this->vertexPositions[1] = 0; 
	this->vertexPositions[2] = 1.f;

	vnum++;

	GLfloat latstep = 180.f / numlats;
	GLfloat longstep = 360.f / numlongs;
	GLfloat longLimit = 180.f;
	GLfloat longStart = -180.f;

	/* Check whether full sphere needs to be drawed or just half of it. */
	if (this->halfSphere)
	{
		longstep  = 180.f / (numlongs-1);
		longLimit = 180.0f;
		longStart = 0.0f;
	}

	/* Define vertices along latitude lines */
	GLfloat lon = 0.0f;
	for (GLfloat lat = 90.f - latstep; lat > -90.f; lat -= latstep)
	{

		int k = 0;
		lat_radians = lat * DEG_TO_RADIANS;
		for ( lon = longStart; k < numlongs; lon += longstep)
		{
			lon_radians = lon * DEG_TO_RADIANS;

			x = cos(lat_radians) * cos(lon_radians);
			y = cos(lat_radians) * sin(lon_radians);
			z = sin(lat_radians);

			/* Define the vertex */
			this->vertexPositions[vnum * 3] = x; this->vertexPositions[vnum * 3 + 1] = y; this->vertexPositions[vnum * 3 + 2] = z;
			vnum++;
			k++;
		}
	}
	/* Define south pole */
	this->vertexPositions[vnum * 3] = 0; this->vertexPositions[vnum * 3 + 1] = 0; this->vertexPositions[vnum * 3 + 2] = -1.f;
}

/* Draws the sphere form the previously defined vertex and index buffers */
void Sphere::draw()
{
	GLuint i;

	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/* Bind the sphere normals */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	/* BInd the texture data */
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	/* Bind the sphere colours */
	/*glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);*/

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, this->vertexCount);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, this->texID);
		/* Bind the indexed vertex buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);

		/* Draw the north pole regions as a triangle  */
		glDrawElements(GL_TRIANGLE_FAN, numlongs + 2, GL_UNSIGNED_INT, (GLvoid*)(0));

		/* Calculate offsets into the indexed array. Note that we multiply offsets by 4
		because it is a memory offset the indices are type GLuint which is 4-bytes */
		GLuint lat_offset_jump = (numlongs * 2) + 2;
		GLuint lat_offset_start = numlongs + 2;
		GLuint lat_offset_current = lat_offset_start * 4;

		/* Draw the triangle strips of latitudes */
		for (i = 0; i < numlats - 2; i++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
			lat_offset_current += (lat_offset_jump * 4);
		}
		/* Draw the south pole as a triangle fan */
		glDrawElements(GL_TRIANGLE_FAN, numlongs + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
	}
}
/* Set the drawmode for this object. */
void Sphere::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
}