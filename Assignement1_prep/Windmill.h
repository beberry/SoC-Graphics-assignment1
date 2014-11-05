#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <stack>

#include "Cylinder.h"
#include "Sphere.h"

class Windmill
{
private:
	GLfloat height;
	GLfloat baseHeight;
	GLfloat topHeight;
	GLfloat topMaxWidth;
	GLfloat baseRatio;

	GLuint modelID, normalMatrixID;

	Cylinder *baseModel;
	Sphere *topModel;


public:
	Windmill(GLfloat height, GLfloat topMaxWidth, GLfloat baseRatio, GLuint modelID, GLuint normalMatrixID);
	~Windmill();

	void Windmill::createTop();
	void Windmill::createBase();
	void Windmill::createWings();
	void Windmill::draw(glm::mat4 &View, std::stack<glm::mat4> &modelTranslate, std::stack<glm::mat4> &modelScale, std::stack<glm::mat4> &modelRotate);
	void setDrawmode(int drawmode);
};

