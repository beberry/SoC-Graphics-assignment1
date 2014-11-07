/**
	A primitive graphics object - sphere.
	Some code taken from lab4 example. The code was extracted into its own
	class and new functionality was added.

	It is possible to define whether the sphere needs to be full / half-sphere.
	It is possible to define the radiuss of the sphere object.

	@author Ian Martin, Modified by Jekabs Stikanss
	@version 1.0, 03/11/2014
*/
#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


class Sphere
{
private:
	/* Common for graphics objects. */
	GLfloat *vertexPositions;
	glm::vec3 *vertexNormals;
	GLfloat *vertexColours;

	GLuint bufferObject, normalsBufferObject, elementBuffer;

	int drawmode;
	GLuint vertexCount;

	/* Only for this object. */
	GLfloat numlats;
	GLfloat numlongs;
	bool halfSphere;

	GLfloat maxWidth;
	GLfloat maxHeight;


public:
	// Common for graphics objects.
	Sphere(GLfloat maxWidth, GLfloat maxHeight, bool halfSphere);
	~Sphere();

	GLfloat* Sphere::getVertexPositions();
	GLfloat* Sphere::getVerteColours();

	GLuint makeVBO(GLfloat numlats, GLfloat numlongs);
	void makeUnitObject(GLuint numlats, GLuint numlongs);
	void draw();
	void setDrawmode(int drawmode);

	GLfloat angle_x, angle_y, angle_z, angle_inc_x, angle_inc_y, angle_inc_z;

	// Only for this object.
};



