#include "Windmill.h"


Windmill::Windmill(GLuint wingCount, GLfloat height, GLfloat topMaxWidth, GLfloat baseRatio, GLuint modelID, GLuint normalMatrixID)
{
	this->wingCount   = wingCount;
	this->baseRatio   = baseRatio;
	this->height	  = height;
	this->baseHeight  = this->height * this->baseRatio;
	this->topHeight   = this->height - this->baseHeight;
	this->topMaxWidth = topMaxWidth;
	this->wingAngle   = 0.0f;

	/* Prepare for setting this models uniforms */
	this->modelID		 = modelID;
	this->normalMatrixID = normalMatrixID;

	/* Construct the object. */
	this->createTop();
	this->createBase();
	this->createWings();

	this->wings;
	int b = 0;
	
}

Windmill::~Windmill()
{
}

/* Create the roof / "head" object for the windmill. */
void Windmill::createTop()
{
	this->topModel = new Sphere(this->topMaxWidth, this->topHeight, true);
	this->topModel->makeVBO(20.0f, 30.0f);
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
	for (int i = 0; i < this->wingCount; i++)
	{
		WindmillWing *tmpWing = new WindmillWing(3.0f, 0.5f, 0.2f);
		tmpWing->makeVBO(0, 0);
		this->wings.push_back(tmpWing);
	}
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

//	this->topModel->draw();
	//modelTranslate.pop();

	/* Do some transformations with the base of the windmill. */
	//modelTranslate.push(glm::translate(modelTranslate.top(), glm::vec3(0.0, -1.0, 0)));
	model = modelTranslate.top() * modelScale.top() * modelRotate.top();
	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
//	this->baseModel->draw();

	/* Do some transformations with the wings of the windmill. */
	GLfloat wingAngle = 360.f / this->wingCount;

	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;


	for (int i = 0; i < this->wings.size(); i++)
	{
		float currentAngle = (float)i*wingAngle+this->wingAngle;

		/* Calculate the translation values */
		GLfloat activeAngle = std::fmod(std::abs(currentAngle), 360.0);
		GLfloat localAngle = std::fmod(activeAngle, 90.0);

		GLfloat localAngle2 = 0.0f;

		int quadrant = activeAngle/90.0+1; /* Calculate in which quadrant the figure is ( I, II, III, IV ) */

		GLfloat radiuss = 1.0f;
		GLfloat xMove = 0.0f;
		GLfloat yMove = 0.0f;

		GLfloat radians = localAngle * DEG_TO_RADIANS;

		if (quadrant == 1)
		{
			xMove = cos(radians)*radiuss;
			yMove = sin(radians)*radiuss;
		}
		else if (quadrant == 2)
		{
			xMove = -sin(radians)*radiuss;
			yMove = cos(radians)*radiuss;
		}
		else if (quadrant == 3)
		{
			xMove = -cos(radians)*radiuss;
			yMove = -sin(radians)*radiuss;
		}
		else if (quadrant == 4)
		{
			xMove = sin(radians)*radiuss;
			yMove = -cos(radians)*radiuss;
		}


		modelTranslate.push(glm::translate(modelTranslate.top(), glm::vec3(xMove, yMove, 0)));
		modelRotate.push(glm::rotate(modelRotate.top(), currentAngle, glm::vec3(0, 0, 1)));
		model = modelTranslate.top() * modelScale.top() * modelRotate.top();

		gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

		glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(this->normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);
		WindmillWing *wmw = this->wings.at(i);
		wmw->draw();

		modelTranslate.pop();
		modelRotate.pop();
	}
	
}

void Windmill::setDrawmode(int drawmode)
{
	this->baseModel->setDrawmode(drawmode);
	this->topModel->setDrawmode(drawmode);

	for (int i = 0; i < this->wings.size(); i++)
	{
		this->wings.at(i)->setDrawmode(drawmode);
	}
}

void Windmill::setWingAngle(GLfloat angle)
{
	this->wingAngle = angle;
}