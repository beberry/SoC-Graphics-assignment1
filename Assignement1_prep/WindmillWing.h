/**
	A windmill wing object, which basically is a wrapper for the Cube object.

	@author Jekabs Stikanss
	@version 1.0, 05/11/2014
*/
#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/* Custom objects. */
#include "Cube.h"


class WindmillWing
{
private:
	/* Common for graphics objects. */
	GLuint bufferObject, normalsBufferObject, elementBuffer;

	int drawmode;
	GLuint vertexCount;

	/* Only for this object. */
	Cube *base;

	GLfloat width;
	GLfloat height;
	GLfloat thickness;

public:
	// Common for graphics objects.
	WindmillWing(GLfloat width, GLfloat height, GLfloat thickness);
	~WindmillWing();

	GLfloat* WindmillWing::getVertexPositions();
	GLfloat* WindmillWing::getVerteColours();

	GLuint WindmillWing::makeVBO();
	void WindmillWing::makeUnitObject();
	void WindmillWing::draw();
	void WindmillWing::setDrawmode(int drawmode);


	GLfloat angle_x, angle_y, angle_z, angle_inc_x, angle_inc_y, angle_inc_z;

	// Only for this object.
};


