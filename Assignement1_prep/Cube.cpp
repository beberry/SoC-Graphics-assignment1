#include "Cube.h"

Cube::Cube()
{
	/* Define vertices for a cube in 12 triangles */
	GLfloat vertexPositions[] =
	{
		-0.25f, 0.25f, -0.25f, 1.f,
		-0.25f, -0.25f, -0.25f, 1.f,
		0.25f, -0.25f, -0.25f, 1.f,

		0.25f, -0.25f, -0.25f, 1.f,
		0.25f, 0.25f, -0.25f, 1.f,
		-0.25f, 0.25f, -0.25f, 1.f,

		0.25f, -0.25f, -0.25f, 1.f,
		0.25f, -0.25f, 0.25f, 1.f,
		0.25f, 0.25f, -0.25f, 1.f,

		0.25f, -0.25f, 0.25f, 1.f,
		0.25f, 0.25f, 0.25f, 1.f,
		0.25f, 0.25f, -0.25f, 1.f,

		0.25f, -0.25f, 0.25f, 1.f,
		-0.25f, -0.25f, 0.25f, 1.f,
		0.25f, 0.25f, 0.25f, 1.f,

		-0.25f, -0.25f, 0.25f, 1.f,
		-0.25f, 0.25f, 0.25f, 1.f,
		0.25f, 0.25f, 0.25f, 1.f,

		-0.25f, -0.25f, 0.25f, 1.f,
		-0.25f, -0.25f, -0.25f, 1.f,
		-0.25f, 0.25f, 0.25f, 1.f,

		-0.25f, -0.25f, -0.25f, 1.f,
		-0.25f, 0.25f, -0.25f, 1.f,
		-0.25f, 0.25f, 0.25f, 1.f,

		-0.25f, -0.25f, 0.25f, 1.f,
		0.25f, -0.25f, 0.25f, 1.f,
		0.25f, -0.25f, -0.25f, 1.f,

		0.25f, -0.25f, -0.25f, 1.f,
		-0.25f, -0.25f, -0.25f, 1.f,
		-0.25f, -0.25f, 0.25f, 1.f,

		-0.25f, 0.25f, -0.25f, 1.f,
		0.25f, 0.25f, -0.25f, 1.f,
		0.25f, 0.25f, 0.25f, 1.f,

		0.25f, 0.25f, 0.25f, 1.f,
		-0.25f, 0.25f, 0.25f, 1.f,
		-0.25f, 0.25f, -0.25f, 1.f
	};

	GLfloat vertexColours[] = {
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
	};


	
	this->vertexPositions = vertexPositions;
	this->vertexColours   = vertexColours;

	this->vertexPositionsSize = 144;
	this->vertexColoursSize   = 144;

}


Cube::~Cube()
{
}


GLfloat* Cube::getVertexPositions()
{
	return this->vertexPositions;
}

GLfloat* Cube::getVerteColours()
{
	return this->vertexColours;
}

int Cube::getVertexPositionsSize()
{
	return this->vertexPositionsSize;

}
int Cube::getVertexColoursSize()
{
	return this->vertexColoursSize;
}