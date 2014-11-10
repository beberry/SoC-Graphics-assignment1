/**
	A primitive graphics object - cylinder.
	The variables can be modified, so that the object becomes more complex - cone, cylinder which changes it's width.

	@author Jekabs Stikans
	@version 1.0, 01/11/2014
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
#include <iostream>
#include <vector>

#include "TextureManager.h"


class Cylinder
{
private:
	/* Common for graphics objects. */
	std::vector<GLfloat> vertexPositions;
	std::vector<glm::vec3> vertexNormals;

	GLuint bufferObject, normalsBufferObject, elementBuffer, textureBuffer, textureID, texID;

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
	Cylinder(GLfloat height, GLfloat maxBottomRadius, GLfloat radiussCoeff, GLuint textureID);
	~Cylinder();

	std::vector<GLfloat>* Cylinder::getVertexPositions();

	GLuint makeVBO(GLfloat numlats, GLfloat numlongs);
	void makeUnitObject(GLuint numlats, GLuint numlongs);
	void draw();
	void setDrawmode(int drawmode);

	GLfloat angle_x, angle_y, angle_z, angle_inc_x, angle_inc_y, angle_inc_z;

	// Only for this object.
	void setTexture(std::string textureName);
};

