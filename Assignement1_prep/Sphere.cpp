#include "Sphere.h"

/* The original code for this object has been taken from Ian Martins lab4 example, modified by Jekabs Stikans */

Sphere::Sphere(GLfloat maxWidth, GLfloat maxHeight, bool halfSphere)
{
	this->maxWidth = maxWidth;
	this->maxHeight = maxHeight;
	this->drawmode = 3;
	this->halfSphere = halfSphere;
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

	/* Store the colours in a buffer object */
	/*glGenBuffers(1, &this->vertexColours);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* this->vertexCount * 4, this->vertexColours, GL_STATIC_DRAW);
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
	this->vertexPositions[0] = 0; this->vertexPositions[1] = 0; this->vertexPositions[2] = 1.f;
	vnum++;

	GLfloat latstep = 180.f / numlats;
	GLfloat longstep = 0.f;
	GLfloat longLimit = 0.f;

	/* Check whether full sphere needs to be drawed or just half of it. */
	if (this->halfSphere)
	{
		longstep = 180.f / numlongs;
		longLimit = 0.0f;
	}
	else
	{
		longstep = 360.f / numlongs;
		longLimit = -180.f;
	}

	/* Define vertices along latitude lines */
	for (GLfloat lat = 90.f - latstep; lat > -90.f; lat -= latstep)
	{
		lat_radians = lat * DEG_TO_RADIANS;
		for (GLfloat lon = 180.f; lon > longLimit; lon-= longstep)
		{
			lon_radians = lon * DEG_TO_RADIANS;

			x = cos(lat_radians) * cos(lon_radians) * this->maxWidth;
			y = cos(lat_radians) * sin(lon_radians) * this->maxHeight;
			z = sin(lat_radians) * this->maxWidth;

			/* Define the vertex */
			this->vertexPositions[vnum * 3] = x; this->vertexPositions[vnum * 3 + 1] = y; this->vertexPositions[vnum * 3 + 2] = z;
			vnum++;
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


void Sphere::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
}
