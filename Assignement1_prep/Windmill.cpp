/**
	Windmill object whic manages different windmill parts, their construction,
	transformations and other related actions.

	@author Jekabs Stikans
	@version 1.0, 30/10/2014
*/
#include "Windmill.h"

/* The constructor method which initializes this object. */
Windmill::Windmill(GLuint wingCount, GLfloat height, GLfloat topMaxWidth, GLfloat baseRatio, GLfloat baseRadiussCoeff, GLuint modelID, GLuint normalMatrixID, GLuint textureID, GLuint textureModeId, GLuint specularModeID)
{
	this->wingCount			  = wingCount;
	this->baseRatio			  = baseRatio;
	this->height			  = height;
	this->baseHeight		  = this->height * this->baseRatio;
	this->topHeight			  = this->height - this->baseHeight;
	this->topMaxWidth	      = topMaxWidth;
	this->wingAngle			  = 0.0f;
	this->wingDistanceRadiuss = 0.3f;
	this->wingHolderEdges	  = 30;
	this->baseLongs			  = 30;
	this->baseRadiussCoeff	  = baseRadiussCoeff;

	/* Prepare for setting this models uniforms */
	this->modelID		 = modelID;
	this->normalMatrixID = normalMatrixID;
	this->textureID		 = textureID;
	this->textureModeId  = textureModeId;
	this->specularModeID = specularModeID;

	/* Construct the object. */
	this->createTop();
	this->createBase();
	this->createBaseCap();
	this->createWingHolder();
	this->createWingHolderCap();
	this->createWings();
}

Windmill::~Windmill()
{
}

/* Create the roof / "head" object for the windmill. */
void Windmill::createTop()
{
	this->topModel = new Sphere(this->topMaxWidth, this->topHeight, true, this->textureID);
	this->topModel->setTexture("copper_3.jpg");
	this->topModel->makeVBO(20.0f, 30.0f);
}

/* Create the base object for the windmill. */
void Windmill::createBase()
{
	this->baseModel = new Cylinder(this->baseHeight, this->topMaxWidth*0.85, this->baseRadiussCoeff, this->textureID);
	this->baseModel->setTexture("index4.jpg");
	this->baseModel->makeVBO(5.0f, this->baseLongs);
}

/* Create the base cap. */
void Windmill::createBaseCap()
{
	this->baseModelCap = new Disk(this->wingDistanceRadiuss, this->baseLongs, this->textureID);
	this->baseModelCap->setTexture("stone_1.jpg");

	/* Get the vertex positions for this cap */
	std::vector<GLfloat>* bVertexPositions = this->baseModel->getVertexPositions();
	std::vector<GLfloat> tmpVector;

	int index = bVertexPositions->size() - this->baseLongs * 3;

	for (int i = 0; i < this->baseLongs * 3; i++)
	{
		if ((i + 2) % 3 == 0)
		{
			/* This is the y value. */
			tmpVector.push_back(0.0);
		}
		else
		{
			/* X or Z value */
			tmpVector.push_back(bVertexPositions->at(index+i));
		}
	}

	this->baseModelCap->setVertexPositions(tmpVector, -1);
	this->baseModelCap->makeVBO();
}

/* Create the cylinder objec which connects the wings witht he head of the windmill. */
void Windmill::createWingHolder()
{
	this->wingHolder = new Cylinder(this->topMaxWidth, this->wingDistanceRadiuss, 1.0, this->textureID);
	this->wingHolder->setTexture("metal_1.jpg");
	this->wingHolder->makeVBO(2.0f, this->wingHolderEdges);
}

/* Create the wing holder cap. */
void Windmill::createWingHolderCap()
{
	this->wingHolderCap = new Disk(this->wingDistanceRadiuss, this->wingHolderEdges, this->textureID);
	this->wingHolderCap->setTexture("metal_1.jpg");

	/* Get the vertex positions for this cap */
	std::vector<GLfloat>* whVertexPositions = this->wingHolder->getVertexPositions();
	std::vector<GLfloat> tmpVector;

	for (int i = 0; i < this->wingHolderEdges * 3; i++)
	{
		if ((i + 2) % 3 == 0)
		{
			/* This is the y value. */
			tmpVector.push_back(0.0);
		}
		else
		{
			/* X or Z value */
			tmpVector.push_back(whVertexPositions->at(i));
		}
	}

	this->wingHolderCap->setVertexPositions(tmpVector, 1);
	this->wingHolderCap->makeVBO();
}

/* Create the wings for the windmill. */
void Windmill::createWings()
{
	for (int i = 0; i < this->wingCount; i++)
	{
		WindmillWing *tmpWing = new WindmillWing(1.0f, 0.25f, 0.025f);
		tmpWing->makeVBO();

		this->wings.push_back(tmpWing);
	}
}

/* Draw the windmill object. */
void Windmill::draw(glm::mat4 &View, std::stack<glm::mat4> &modelTranslate, std::stack<glm::mat4> &modelScale, std::stack<glm::mat4> &modelRotate)
{
	glm::mat4 model			  = glm::mat4(1.0f);
	glm::mat3 gl_NormalMatrix = glm::mat3(1.0f);

	/* Rotate all arms aroudn y axis*/
	glm::mat4 fullRotation = glm::mat4(1.0f);
	fullRotation = glm::rotate(fullRotation, this->headAngle, glm::vec3(0, 1, 0));

	/* Do some transformations with the roof of the windmill. */
	modelRotate.push(glm::rotate(modelRotate.top(), this->headAngle, glm::vec3(0, 1, 0)));
	model = modelTranslate.top() * modelScale.top() * modelRotate.top();
	
	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

	glUniform1ui(this->textureModeId, 1);
	glUniform1ui(this->specularModeID, 1);
	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(this->normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);

	this->topModel->draw();
	modelRotate.pop();
	glUniform1ui(this->specularModeID, 0);
	glUniform1ui(this->textureModeId, 0);

	/* Do some transformations with the cylinder which connects the sails to the roof of the windmill. */
	modelRotate.push(glm::rotate(modelRotate.top(), 90.0f, glm::vec3(1, 0, 0)));
	modelRotate.push(glm::rotate(modelRotate.top(), this->wingAngle, glm::vec3(0, 1, 0)));
	modelTranslate.push(glm::translate(modelTranslate.top(), glm::vec3(0, 0.15, this->topMaxWidth/2)));
	model = fullRotation*modelTranslate.top() * modelScale.top() * modelRotate.top();

	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

	glUniform1ui(this->textureModeId, 1);
	glUniform1ui(this->specularModeID, 1);
	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(this->normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);

	this->wingHolder->draw();
	modelRotate.pop();
	modelRotate.pop();
	modelTranslate.pop();

	glUniform1ui(this->specularModeID, 0);
	glUniform1ui(this->textureModeId, 0);

	/* Do some transformations with the wingholder cap. */
	modelRotate.push(glm::rotate(modelRotate.top(), 90.0f, glm::vec3(1, 0, 0)));
	modelRotate.push(glm::rotate(modelRotate.top(), this->wingAngle, glm::vec3(0, 1, 0)));
	modelTranslate.push(glm::translate(modelTranslate.top(), glm::vec3(0, 0.15, this->topMaxWidth / 2)));
	model = fullRotation*modelTranslate.top() * modelScale.top() * modelRotate.top();

	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

	glUniform1ui(this->textureModeId, 1);
	glUniform1ui(this->specularModeID, 1);
	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(this->normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);

	this->wingHolderCap->draw();
	modelRotate.pop();
	modelRotate.pop();
	modelTranslate.pop();

	glUniform1ui(this->specularModeID, 0);
	glUniform1ui(this->textureModeId, 0);


	/* Do some transformations with the base of the windmill. */
	model = modelTranslate.top() * modelScale.top() * modelRotate.top();

	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

	glUniform1ui(this->textureModeId, 1);
	glUniform1ui(this->specularModeID,0);
	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(this->normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);
	glUniform1ui(this->specularModeID, 0);

	this->baseModel->draw();
	glUniform1ui(this->textureModeId, 0);

	/* Do some transformations with the base cap. */
	glm::mat4 move = glm::mat4(1.0f);
	move = glm::translate(move, glm::vec3(0, -this->baseHeight, 0));

	model = modelTranslate.top() * modelScale.top() * move * modelRotate.top();

	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

	glUniform1ui(this->textureModeId, 1);
	glUniform1ui(this->specularModeID, 0);
	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(this->normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);

	this->baseModelCap->draw();
	glUniform1ui(this->specularModeID, 0);
	glUniform1ui(this->textureModeId, 0);

	/* Do some transformations with the wings of the windmill. */
	GLfloat wingAngle = 360.f / this->wingCount;

	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;

	/* Apply all transformations to each wing of the windmill. */
	for (int i = 0; i < this->wings.size(); i++)
	{
		/* This could have been done in a simpler way by just doing transformations in a different order. */
		float currentAngle = (float)i*wingAngle+this->wingAngle;

		if (currentAngle < 0)
		{
			currentAngle += 360.0f;
		}

		/* Calculate the translation values */
		GLfloat activeAngle = std::fmod(std::abs(currentAngle), 360.0);
		GLfloat localAngle  = std::fmod(activeAngle, 90.0);

		GLfloat localAngle2 = 0.0f;

		int quadrant = abs(activeAngle/90.0)+1; /* Calculate in which quadrant the figure is ( I, II, III, IV ) */

		GLfloat xMove = 0.0f;
		GLfloat yMove = 0.0f;

		GLfloat radians = localAngle * DEG_TO_RADIANS;

		if (quadrant == 1)
		{
			xMove = cos(radians)*this->wingDistanceRadiuss;
			yMove = sin(radians)*this->wingDistanceRadiuss;
		}
		else if (quadrant == 2)
		{
			xMove = -sin(radians)*this->wingDistanceRadiuss;
			yMove = cos(radians)*this->wingDistanceRadiuss;
		}
		else if (quadrant == 3)
		{
			xMove = -cos(radians)*this->wingDistanceRadiuss;
			yMove = -sin(radians)*this->wingDistanceRadiuss;
		}
		else if (quadrant == 4)
		{
			xMove = sin(radians)*this->wingDistanceRadiuss;
			yMove = -cos(radians)*this->wingDistanceRadiuss;
		}


		modelTranslate.push(glm::translate(modelTranslate.top(), glm::vec3(xMove, yMove+0.15, 0.46)));
		modelRotate.push(glm::rotate(modelRotate.top(), currentAngle, glm::vec3(0, 0, 1)));
		model = fullRotation*modelTranslate.top() * modelScale.top() * modelRotate.top();

		gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

		glUniform1ui(this->specularModeID, 0);
		glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(this->normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);

		WindmillWing *wmw = this->wings.at(i);
		wmw->draw();

		modelTranslate.pop();
		modelRotate.pop();
		glUniform1ui(this->specularModeID, 0);
	}
}

/* Set the drawmode for this object and its child elements. */
void Windmill::setDrawmode(int drawmode)
{
	this->baseModel->setDrawmode(drawmode);
	this->baseModelCap->setDrawmode(drawmode);
	this->wingHolder->setDrawmode(drawmode);
	this->wingHolderCap->setDrawmode(drawmode);
	this->topModel->setDrawmode(drawmode);

	for (int i = 0; i < this->wings.size(); i++)
	{
		this->wings.at(i)->setDrawmode(drawmode);
	}
}

/* Set the angle of windmill wings. */
void Windmill::setWingAngle(GLfloat angle)
{
	this->wingAngle = angle;
}

/* Set the angle around Y axis for the "head/hat" of the windmill and the windmill wings.*/
void Windmill::setHeadAngle(GLfloat angle)
{
	this->headAngle = angle;
}