#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

class Cube
{
private:
	GLfloat *vertexPositions;
	GLfloat *vertexColours;
	int vertexPositionsSize;
	int vertexColoursSize;
	

public:
	Cube();
	~Cube();

	GLfloat* Cube::getVertexPositions();
	GLfloat* Cube::getVerteColours();

	int Cube::getVertexPositionsSize();
	int Cube::getVertexColoursSize();
};