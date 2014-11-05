#include "Cylinder.h"

/* Inlcude some standard headers */
#include <iostream>

/* Construction */
Cylinder::Cylinder(GLfloat height, GLfloat maxTopRadius)
{
	this->radiussCoeff = 1.12;
	this->height		 = height;
	this->maxTopRadius = maxTopRadius;
	this->drawmode	     = 3;
	this->vertexCount    = 0;
}

/* Destructor */
Cylinder::~Cylinder()
{
}

/* Create the vertex buffer object */
GLuint Cylinder::makeVBO(GLfloat numlats, GLfloat numlongs)
{
	GLuint i, j;

	this->numlats  = numlats;
	this->numlongs = numlongs;

	/* Calculate the number of vertices required in hemisphere */
	this->vertexCount = (numlats) * (numlongs);

	this->vertexPositions = new GLfloat[this->vertexCount * 3];
	this->vertexColours   = new GLfloat[this->vertexCount * 4];

	makeUnitObject(numlats, numlongs);


	this->vertexNormals = new glm::vec3[this->vertexCount];



	// init arr
	for (int i = 0; i < this->vertexCount; i++)
	{
		this->vertexNormals[i] = glm::vec3(0.0, 0.0, 0.0);
	}


	// -1 to ignore the last point in the strip. We have to ignore first and last point in the strip.
	
	int ind = 0;
	
	for (int lat = 0; lat < numlats-1; lat++)
	{
		for (int lon = 0; lon < numlongs; lon++)
		{
			int p0Index = 0;
			int p1Index = 0;
			int p2Index = 0;

			glm::vec3 crossP = glm::cross(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

			if ((ind+1) % 2 == 0)
			{
				// Triangle looks like this
				// p2*------* p1
				//    \    |
				//     \   |
				//      \  |
				//       \ |
				//        \|
				//         * p0 current point, so the other two points are one lat above.
				p0Index = ind;
				p1Index = ind + numlongs;
				p2Index = ind + 1;


				glm::vec3 p0 = glm::vec3(
					this->vertexPositions[p0Index * 3],
					this->vertexPositions[p0Index * 3 + 1],
					this->vertexPositions[p0Index * 3 + 2]
				);

				glm::vec3 p1 = glm::vec3(
					this->vertexPositions[p1Index * 3],
					this->vertexPositions[p1Index * 3 + 1],
					this->vertexPositions[p1Index * 3 + 2]
				);

				glm::vec3 p2 = glm::vec3(
					this->vertexPositions[p2Index * 3],
					this->vertexPositions[p2Index * 3 + 1],
					this->vertexPositions[p2Index * 3 + 2]
				);


				glm::vec3 vecA = p1 - p0;
				glm::vec3 vecB = p2 - p0;

				crossP = glm::cross(vecB, vecA);
			}
			else
			{
				// Triangle looks like this
				//    * p2
				//    |\
				//    | \
				//    |  \
				//    |   \
				//    |    \
				//  p0*------* p1   p0 - current point

				p0Index = ind;
				p1Index = ind + 1;
				p2Index = ind + numlongs;


				glm::vec3 p0 = glm::vec3(
					this->vertexPositions[p0Index * 3],
					this->vertexPositions[p0Index * 3 + 1], 
					this->vertexPositions[p0Index * 3 + 2]
				);

				glm::vec3 p1 = glm::vec3(
					this->vertexPositions[p1Index * 3], 
					this->vertexPositions[p1Index * 3 + 1], 
					this->vertexPositions[p1Index * 3 + 2]
				);

				glm::vec3 p2 = glm::vec3(
					this->vertexPositions[p2Index * 3], 
					this->vertexPositions[p2Index * 3 + 1], 
					this->vertexPositions[p2Index * 3 + 2]
				);

				glm::vec3 vecA = p1 - p0;
				glm::vec3 vecB = p2 - p0;

				crossP = glm::cross(vecA, vecB);
			}


			// Set the normals
			this->vertexNormals[p0Index] += crossP;
			this->vertexNormals[p1Index] += crossP;
			this->vertexNormals[p2Index] += crossP;
				

			if (ind % ((int)numlongs + 1) == 0)
			{
				// First triangle in this strip.
				this->vertexNormals[ind + (int)numlongs * 2 - 1] += crossP;
				this->vertexNormals[ind + (int)numlongs - 1] += crossP;
			}

			if ((ind + 1) % (int)numlongs == 0)
			{
				// Last triangle in this strip.
				this->vertexNormals[ind - (int)numlongs + 1] += crossP;
				this->vertexNormals[ind + 1] += crossP;

			}

			ind++;
		}
	}
	
	// Normalize the normals
	for (int i = 0; i < this->vertexCount; i++)
	{
		this->vertexNormals[i] = glm::normalize(this->vertexNormals[i]);
		//std::cout << "x" << this->vertexNormals[i].x << " y" << this->vertexNormals[i].y << " z" << this->vertexNormals[i].z << std::endl;
	}


	/* Define colours as the x,y,z components of the sphere vertices */
	for (i = 0; i < this->vertexCount; i++)
	{
		this->vertexColours[i * 4]     = this->vertexPositions[i * 3];
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*this->vertexCount, this->vertexNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	/* Store the colours in a buffer object */
	//glGenBuffers(1, &this->normals);
	//glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pColours, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Calculate the number of indices in our index array and allocate memory for it */
	GLuint numindices = (numlongs*2+2) * (numlats-1);

	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index
	GLuint start = 0;		// Start index for each latitude row

	for (j = 0; j < numlats-1; j++)
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

	// Generate a buffer for the indices
	glGenBuffers(1, &this->elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	delete pindices;
	return this->vertexCount;
}

void Cylinder::makeUnitObject(GLuint numlats, GLuint numlongs)
{
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLuint vnum = 0;
	GLfloat x, y, z, lat_radians, lon_radians;

	GLfloat latstep = this->height / (numlats);
	GLfloat longstep = 360.f / numlongs;

	int latI = 0;

	GLfloat radiuss = this->maxTopRadius;

	for (GLfloat i = 0; latI < numlats; i -= latstep)
	{
		latI++;

		/* Define vertices along latitude lines */
		int p = 0;

		for (GLfloat lon = -180.f; lon < 180.f, p < numlongs; lon += longstep)
		{
			lon_radians = (lon)* DEG_TO_RADIANS;

			x = cos(lon_radians)*radiuss;
			y = i;
			z = sin(lon_radians)*radiuss;


			/* Define the vertex */
			this->vertexPositions[vnum * 3]     = x; 
			this->vertexPositions[vnum * 3 + 1] = y; 
			this->vertexPositions[vnum * 3 + 2] = z;
			
			vnum++;
			p++;
		}

		radiuss = radiuss * this->radiussCoeff;
	}
}

void Cylinder::draw()
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
	//glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (this->drawmode == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (this->drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, this->vertexCount);
	}
	else
	{
		/* Draw the latitude triangle strips */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);

		GLuint lat_offset = 4 * (this->numlongs * 2 + 2);

		for (i = 0; i < numlats; i++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, this->numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset*i));
		}
	}
}

void Cylinder::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
}


