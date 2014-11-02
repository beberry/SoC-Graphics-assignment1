#include "WindmillBase.h"

/* Inlcude some standard headers */
#include <iostream>

/* Construction */
WindmillBase::WindmillBase(GLfloat height)
{
	this->topRadiusCoeff = 0.7;
	this->height		 = 2;
	this->bottomRadius	 = 2;
	this->drawmode	     = 3;
	this->vertexCount    = 0;
}

/* Destructor */
WindmillBase::~WindmillBase()
{
}

/* Create the vertex buffer object */
GLuint WindmillBase::makeVBO(GLfloat numlats, GLfloat numlongs)
{
	GLuint i, j;

	this->numlats  = numlats;
	this->numlongs = numlongs;

	/* Calculate the number of vertices required in hemisphere */
	this->vertexCount = (numlats) * (numlongs);

	GLfloat* pVertices = new GLfloat[this->vertexCount * 3];
	GLfloat* pColours = new GLfloat[this->vertexCount * 4];

	makeUnitObject(pVertices, numlats, numlongs);


	glm::vec3* pNormals = new glm::vec3[this->vertexCount];
	GLfloat* normalsDivisors = new GLfloat[this->vertexCount];



	// init arr
	for (int i = 0; i < this->vertexCount; i++)
	{
		pNormals[i] = glm::vec3(0.0, 0.0, 0.0);
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

			//if (!(ind % 2 == 0 && lat != numlats - 1))
			{
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


					glm::vec3 p0 = glm::vec3(pVertices[p0Index * 3], pVertices[p0Index * 3 + 1], pVertices[p0Index * 3 + 2]); // Current vertice
					glm::vec3 p1 = glm::vec3(pVertices[p1Index * 3], pVertices[p1Index * 3 + 1], pVertices[p1Index * 3 + 2]);
					glm::vec3 p2 = glm::vec3(pVertices[p2Index * 3], pVertices[p2Index * 3 + 1], pVertices[p2Index * 3 + 2]);


					glm::vec3 vecA = p1 - p0;
					glm::vec3 vecB = p2 - p0;

					crossP = glm::cross(vecA, vecB);
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


					glm::vec3 p0 = glm::vec3(pVertices[p0Index * 3], pVertices[p0Index * 3 + 1], pVertices[p0Index * 3 + 2]); // Current vertice
					glm::vec3 p1 = glm::vec3(pVertices[p1Index * 3], pVertices[p1Index * 3 + 1], pVertices[p1Index * 3 + 2]);
					glm::vec3 p2 = glm::vec3(pVertices[p2Index * 3], pVertices[p2Index * 3 + 1], pVertices[p2Index * 3 + 2]);



					GLfloat aa = pVertices[p1Index * 3 + 2];
					glm::vec3 vecA = p1 - p0;
					glm::vec3 vecB = p2 - p0;

					crossP = glm::cross(vecB, vecA);
				}


				// Set the normals
				pNormals[p0Index] += crossP;
				pNormals[p1Index] += crossP;
				pNormals[p2Index] += crossP;
				

				if (ind % (lon + 1) == 0)
				{
					// First triangle in this strip.
					pNormals[ind + (int)numlongs*2 - 1] += crossP;
					pNormals[ind + (int)numlongs - 1] += crossP;
				}

				if ((ind + 1) % (int)numlongs == 0)
				{
					// Last triangle in this strip.
					pNormals[ind - (int)numlongs + 1] += crossP;
					pNormals[ind + 1] += crossP;

				}
			}

			ind++;
		}
	}
	
	// Normalize the normals
	for (int i = 0; i < this->vertexCount; i++)
	{
		pNormals[i] = glm::normalize(pNormals[i]);
		std::cout << "x" << pNormals[i].x << " y" << pNormals[i].y << " z" << pNormals[i].z << std::endl;
	}


	/* Define colours as the x,y,z components of the sphere vertices */
	for (i = 0; i < this->vertexCount; i++)
	{
		pColours[i * 4] = pVertices[i * 3];
		pColours[i * 4 + 1] = pVertices[i * 3 + 1];
		pColours[i * 4 + 2] = pVertices[i * 3 + 2];
		pColours[i * 4 + 3] = 1.f;
	}

	/* Generate the vertex buffer object */
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->vertexCount * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &this->sphereNormals);
	glBindBuffer(GL_ARRAY_BUFFER, this->sphereNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*this->vertexCount, pNormals, GL_STATIC_DRAW);
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
	delete pColours;
	delete pVertices;

	return this->vertexCount;
}

void WindmillBase::makeUnitObject(GLfloat *pVertices, GLuint numlats, GLuint numlongs)
{
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLuint vnum = 0;
	GLfloat x, y, z, lat_radians, lon_radians;

	GLfloat latstep = this->height / numlats;
	GLfloat longstep = 360.f / numlongs;

	for (GLfloat i = -this->height / 2.0f; i < this->height/2; i += latstep)
	{
		if (vnum > 0)
		{
			this->bottomRadius = this->bottomRadius*this->topRadiusCoeff;
		}

		/* Define vertices along latitude lines */
		int p = 0;

		for (GLfloat lon = -180.f; lon < 180.f, p < numlongs; lon += longstep)
		{
			lon_radians = (lon)* DEG_TO_RADIANS;

			x = cos(lon_radians)*this->bottomRadius;
			y = i;
			z = sin(lon_radians)*this->bottomRadius;


			/* Define the vertex */
			pVertices[vnum * 3] = x; pVertices[vnum * 3 + 1] = y; pVertices[vnum * 3 + 2] = z;
			
			vnum++;
			p++;
		}
	}
}

void WindmillBase::draw()
{
	GLuint i;

	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/* Bind the sphere normals */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->sphereNormals);
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

void WindmillBase::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
}


