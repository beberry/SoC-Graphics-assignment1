/**
	A windmill wing object, which basically is a wrapper for the Cube object.

	@author Jekabs Stikanss
	@version 1.0, 05/11/2014
*/
#include "WindmillWing.h"


WindmillWing::WindmillWing(GLfloat width, GLfloat height, GLfloat thickness)
{
	this->width		= width;
	this->height	= height;
	this->thickness = thickness;

	this->base = new Cube();
}

WindmillWing::~WindmillWing()
{
}


/* Create the vertex buffer object */
GLuint WindmillWing::makeVBO()
{
	makeUnitObject();

	return 0;
}

/* Calculate the vertex positions and pass them to the Cube primitive. */
void WindmillWing::makeUnitObject()
{
	/* Calculate the vertex positions for this object. */
	std::vector<GLfloat> basePoints =
	{
		-this->width / 2, this->height / 2, -this->thickness/2,
		-this->width / 2, -this->height / 2, -this->thickness/2,
		this->width / 2, -this->height / 2, -this->thickness/2,

		this->width / 2, -this->height / 2, -this->thickness/2,
		this->width / 2, this->height / 2, -this->thickness/2,
		-this->width / 2, this->height / 2, -this->thickness/2,

		this->width / 2, -this->height / 2, -this->thickness/2,
		this->width / 2, -this->height / 2, this->thickness/2,
		this->width / 2, this->height / 2, -this->thickness/2,

		this->width / 2, -this->height / 2, this->thickness/2,
		this->width / 2, this->height / 2, this->thickness/2,
		this->width / 2, this->height / 2, -this->thickness/2,

		this->width / 2, -this->height / 2, this->thickness/2,
		-this->width / 2, -this->height / 2, this->thickness/2,
		this->width / 2, this->height / 2, this->thickness/2,

		-this->width / 2, -this->height / 2, this->thickness/2,
		-this->width / 2, this->height / 2, this->thickness/2,
		this->width / 2, this->height / 2, this->thickness/2,

		-this->width / 2, -this->height / 2, this->thickness/2,
		-this->width / 2, -this->height / 2, -this->thickness/2,
		-this->width / 2, this->height / 2, this->thickness/2,

		-this->width / 2, -this->height / 2, -this->thickness/2,
		-this->width / 2, this->height / 2, -this->thickness/2,
		-this->width / 2, this->height / 2, this->thickness/2,

		-this->width / 2, -this->height / 2, this->thickness/2,
		this->width / 2, -this->height / 2, this->thickness/2,
		this->width / 2, -this->height / 2, -this->thickness/2,

		this->width / 2, -this->height / 2, -this->thickness/2,
		-this->width / 2, -this->height / 2, -this->thickness/2,
		-this->width / 2, -this->height / 2, this->thickness/2,

		-this->width / 2, this->height / 2, -this->thickness/2,
		this->width / 2, this->height / 2, -this->thickness/2,
		this->width / 2, this->height / 2, this->thickness/2,

		this->width / 2, this->height / 2, this->thickness/2,
		-this->width / 2, this->height / 2, this->thickness/2,
		-this->width / 2, this->height / 2, -this->thickness/2
	};

	/* Update the child objects vertex positions. */
	this->base->setVertexPositions(basePoints);
	this->base->makeVBO();
}

/* Draw this object. */
void WindmillWing::draw()
{
	this->base->draw();
}

/* Set the draw mode for this object and its childs. */
void WindmillWing::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
	this->base->setDrawmode(drawmode);
}



