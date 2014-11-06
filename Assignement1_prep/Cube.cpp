#include "Cube.h"


Cube::Cube()
{
	this->drawmode = 3;

	/* Define vertices for a cube in 12 triangles */
	this->vertexPositions =
	{
		-0.25f, 0.25f, -0.25f, 
		-0.25f, -0.25f, -0.25f, 
		0.25f, -0.25f, -0.25f, 

		0.25f, -0.25f, -0.25f, 
		0.25f, 0.25f, -0.25f, 
		-0.25f, 0.25f, -0.25f, 

		0.25f, -0.25f, -0.25f, 
		0.25f, -0.25f, 0.25f, 
		0.25f, 0.25f, -0.25f, 

		0.25f, -0.25f, 0.25f, 
		0.25f, 0.25f, 0.25f, 
		0.25f, 0.25f, -0.25f, 

		0.25f, -0.25f, 0.25f, 
		-0.25f, -0.25f, 0.25f, 
		0.25f, 0.25f, 0.25f, 

		-0.25f, -0.25f, 0.25f, 
		-0.25f, 0.25f, 0.25f, 
		0.25f, 0.25f, 0.25f, 

		-0.25f, -0.25f, 0.25f, 
		-0.25f, -0.25f, -0.25f, 
		-0.25f, 0.25f, 0.25f, 

		-0.25f, -0.25f, -0.25f, 
		-0.25f, 0.25f, -0.25f, 
		-0.25f, 0.25f, 0.25f, 

		-0.25f, -0.25f, 0.25f, 
		0.25f, -0.25f, 0.25f, 
		0.25f, -0.25f, -0.25f, 

		0.25f, -0.25f, -0.25f, 
		-0.25f, -0.25f, -0.25f, 
		-0.25f, -0.25f, 0.25f, 

		-0.25f, 0.25f, -0.25f, 
		0.25f, 0.25f, -0.25f, 
		0.25f, 0.25f, 0.25f, 

		0.25f, 0.25f, 0.25f, 
		-0.25f, 0.25f, 0.25f, 
		-0.25f, 0.25f, -0.25f
	};

	GLfloat vertexColours[] = {
		-0.25f, 0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, 0.25f,

		-0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, 0.25f,

		0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, -0.25f,
	};

	this->vertexNormals = 
	{
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
	};


	
	this->vertexColours   = vertexColours;
	int k = 0;
	/* Generate the vertex buffer object 
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexPositions->size()*sizeof(GLfloat), this->vertexPositions->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object 
	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), this->vertexNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	int b = this->vertexPositions->size();
	this->kk = 2;*/
}


Cube::~Cube()
{
}

GLuint Cube::makeVBO()
{


	return 0;
}


std::vector<GLfloat>* Cube::getVertexPositions()
{
	return &this->vertexPositions;
}

GLfloat* Cube::getVerteColours()
{
	return this->vertexColours;
}

void Cube::setVertexPositions(std::vector<GLfloat> vertexPositions)
{
	this->vertexPositions = vertexPositions;
	//glClearBufferData(this->bufferObject, );
	/*Generate the vertex buffer object */
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexPositions.size()*sizeof(GLfloat), this->vertexPositions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->vertexNormals;
	/* Store the normals in a buffer object */
	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexNormals.size()*sizeof(GLfloat), this->vertexNormals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::draw()
{
	this->vertexPositions;

	/* Bind cube vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube normals. Note that this is in attribute index 2 */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

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
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}