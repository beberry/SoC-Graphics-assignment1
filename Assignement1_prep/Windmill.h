/**
	Windmill object whic manages different windmill parts, their construction,
	transformations and other related actions.

	@author Jekabs Stikans
	@version 1.0, 30/10/2014
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
#include <stack>
#include <vector>
#include <cmath> 

/* Custom objects. */
#include "Cylinder.h"
#include "Sphere.h"
#include "WindmillWing.h"
#include "Cube.h"

class Windmill
{
private:
	GLfloat height;
	GLfloat baseHeight;
	GLfloat topHeight;
	GLfloat topMaxWidth;
	GLfloat baseRatio;
	GLfloat baseRadiussCoeff;

	GLfloat wingAngle;
	GLfloat headAngle;

	GLuint modelID, normalMatrixID;

	GLuint wingCount;
	Cylinder *baseModel;
	Sphere *topModel;
	std::vector <WindmillWing*> wings;


public:
	Windmill(GLuint wingCount, GLfloat height, GLfloat topMaxWidth, GLfloat baseRatio, GLfloat baseRadiussCoeff, GLuint modelID, GLuint normalMatrixID);
	~Windmill();

	void Windmill::createTop();
	void Windmill::createBase();
	void Windmill::createWings();
	void Windmill::draw(glm::mat4 &View, std::stack<glm::mat4> &modelTranslate, std::stack<glm::mat4> &modelScale, std::stack<glm::mat4> &modelRotate);
	void Windmill::setDrawmode(int drawmode);
	void Windmill::setWingAngle(GLfloat angle);
	void Windmill::setHeadAngle(GLfloat angle);
};

