#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


class Cylinder
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

	GLfloat height;
	GLfloat maxTopRadius;
	GLfloat radiussCoeff; // used to multiply the bottom radius with this coeff, to get the top radius.

	
public:
	// Common for graphics objects.
	Cylinder(GLfloat height, GLfloat maxBottomRadius);
	~Cylinder();

	GLfloat* Cylinder::getVertexPositions();
	GLfloat* Cylinder::getVerteColours();

	GLuint makeVBO(GLfloat numlats, GLfloat numlongs);
	void makeUnitObject(GLuint numlats, GLuint numlongs);
	void draw();
	void setDrawmode(int drawmode);


	GLfloat angle_x, angle_y, angle_z, angle_inc_x, angle_inc_y, angle_inc_z;

	// Only for this object.
};

