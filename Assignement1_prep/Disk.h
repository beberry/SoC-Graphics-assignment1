/**
	A primitive graphics object - disk.

	@author Jekabs Stikanss
	@version 1.0, 09/11/2014
*/

#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/* Some STL includes. */
#include <vector>

#include "TextureManager.h"

class Disk
{
private:
	/* Common for graphics objects. */
	std::vector<GLfloat> vertexPositions;
	std::vector<glm::vec3> vertexNormals;

	GLuint bufferObject, normalsBufferObject, elementBuffer, textureBuffer, textureID, texID;

	int drawmode;
	GLuint vertexCount;

	/* Only for this object. */
	GLfloat radiuss;
	GLuint numedges;

public:
	// Common for graphics objects.
	Disk(GLfloat radiuss, GLuint numedges, GLuint textureID);
	~Disk();

	std::vector<GLfloat>* Disk::getVertexPositions();

	GLuint Disk::makeVBO();
	void Disk::makeUnitObject();
	void Disk::draw();
	void Disk::setDrawmode(int drawmode);


	GLfloat angle_x, angle_y, angle_z, angle_inc_x, angle_inc_y, angle_inc_z;

	// Only for this object.
	void setVertexPositions(std::vector<GLfloat> vertexPositions, GLint normalYDirection);
	void setTexture(std::string textureName);
};

