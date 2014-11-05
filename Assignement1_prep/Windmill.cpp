#include "Windmill.h"


Windmill::Windmill(GLfloat height, GLfloat topMaxWidth, GLfloat baseRatio, GLuint modelID, GLuint normalMatrixID)
{
	this->baseRatio   = baseRatio;
	this->height	  = height;
	this->baseHeight  = this->height * this->baseRatio;
	this->topHeight   = this->height - this->baseHeight;
	this->topMaxWidth = topMaxWidth;

	/* Prepare for setting this models uniforms */
	this->modelID		 = modelID;
	this->normalMatrixID = normalMatrixID;

	/* Construct the object. */
	this->createTop();
	this->createBase();
	
}

Windmill::~Windmill()
{
}

/* Create the roof / "head" object for the windmill. */
void Windmill::createTop()
{
	this->topModel = new Sphere(this->topMaxWidth, this->topHeight, true);
	this->topModel->makeVBO(10.0f, 4.0f);
}

/* Create the base object for the windmill. */
void Windmill::createBase()
{
	this->baseModel = new Cylinder(this->baseHeight, this->topMaxWidth*0.90);
	this->baseModel->makeVBO(3.0f, 30.0f);
}

/* Create the wings for the windmill. */
void Windmill::createWings()
{

}

/* Draw the windmill object. */
void Windmill::draw(glm::mat4 &View, std::stack<glm::mat4> &modelTranslate, std::stack<glm::mat4> &modelScale, std::stack<glm::mat4> &modelRotate)
{
	glm::mat4 model			  = glm::mat4(1.0f);
	glm::mat3 gl_NormalMatrix = glm::mat3(1.0f);

	/* Do some transformations with the roof of the windmill. */
	//modelTranslate.push(glm::translate(modelTranslate.top(), glm::vec3(0.0, this->topHeight/2, 0)));
	model = modelTranslate.top() * modelScale.top() * modelRotate.top();
	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(this->normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);

	this->topModel->draw();
	//modelTranslate.pop();

	/* Do some transformations with the base of the windmill. */
	//modelTranslate.push(glm::translate(modelTranslate.top(), glm::vec3(0.0, -1.0, 0)));
	model = modelTranslate.top() * modelScale.top() * modelRotate.top();
	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
	this->baseModel->draw();
}

void Windmill::setDrawmode(int drawmode)
{
	this->baseModel->setDrawmode(drawmode);
	this->topModel->setDrawmode(drawmode);
}