#include "Disk.h"


Disk::Disk(GLfloat radiuss, GLuint numedges, GLuint textureID)
{
	this->radiuss = radiuss;
	this->numedges = numedges;
	this->drawmode = 3; 
	
	this->textureID = textureID;

	try
	{
		/* Not actually needed if using one texture at a time */
		glActiveTexture(GL_TEXTURE0);

		/* load an image file directly as a new OpenGL texture */
		this->texID = SOIL_load_OGL_texture("textures/metal_1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if (this->texID == 0)
		{
			printf("TexID SOIL loading error: '%s'\n", SOIL_last_result());
		}
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}

	/* Define the texture behaviour parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Can't get this to work
	glGenerateMipmap(GL_TEXTURE_2D);
}


Disk::~Disk()
{
}

GLuint Disk::makeVBO()
{
	return 0;
}


std::vector<GLfloat>* Disk::getVertexPositions()
{
	return &this->vertexPositions;
}

/*GLfloat* Disk::getVerteColours()
{
	return this->vertexColours;
}*/

/* A method which allows changing the vetex positions of this object. */
void Disk::setVertexPositions(std::vector<GLfloat> vertexPositions)
{
	/* Set the new vertex positions. */
	this->vertexPositions = vertexPositions;

	/* Insert the center vertex */
	this->vertexPositions.insert(this->vertexPositions.begin(), 3, 0.0);

	/* Close the triangle fan */
	this->vertexPositions.push_back(*(this->vertexPositions.begin() + 3));
	this->vertexPositions.push_back(*(this->vertexPositions.begin() + 4));
	this->vertexPositions.push_back(*(this->vertexPositions.begin() + 5));

	/* Set the normals */
	this->vertexNormals.clear();

	for (int i = 0; i < this->vertexPositions.size()/3; i++)
	{
		this->vertexNormals.push_back(glm::vec3(0, 1, 0));
	}

	/* Generate texture coordinats */
	std::vector<GLfloat> texcoords1;

	GLfloat vecLen = glm::vec3(this->vertexPositions.at(3), this->vertexPositions.at(4), this->vertexPositions.at(5)).length();

	GLfloat latstep = 1.0 / this->vertexPositions.size() / 3 - 1;

	texcoords1.push_back(0.5);
	texcoords1.push_back(0.5);

	for (int x = 0; x < this->vertexPositions.size() / 3 - 1; x++)
	{
		GLfloat xval = this->vertexPositions[x * 3];
		GLfloat yval = this->vertexPositions[x * 3+2];

		xval = (xval / vecLen + 1) / 2;
		yval = (yval / vecLen + 1) / 2;

		texcoords1.push_back(xval);
		texcoords1.push_back(yval);
	}

	/* Generate the texture coordinate buffer */
	glGenBuffers(1, &this->textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, texcoords1.size()*sizeof(GLfloat), texcoords1.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*Generate the vertex buffer object */
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexPositions.size()*sizeof(GLfloat), this->vertexPositions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, this->vertexNormals.size()*sizeof(glm::vec3), this->vertexNormals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* Draw cube object. */
void Disk::draw()
{
	GLuint i;

	/* Bind cube vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind the sphere normals */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* BInd the texture data */
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Bind the sphere colours */
	//glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (this->drawmode == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (this->drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, this->vertexCount);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, this->texID);

		/* Draw the latitude triangle strips */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);

		/* Close the bottom of the cylinder with a trianglefan  */
		//glDrawElements(GL_TRIANGLE_FAN, this->numedges, GL_UNSIGNED_INT, (GLvoid*)(0));
		glDrawArrays(GL_TRIANGLE_FAN, 0, this->numedges+2);
	}
}

/* Set the drawmode for this object. */
void Disk::setDrawmode(int drawmode)
{
	this->drawmode = drawmode;
}