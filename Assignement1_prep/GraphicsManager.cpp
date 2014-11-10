/**
	An object which is responsible for managing the whole application - draws the frames,
	reacts to callbacks.

	@author Jekabs Stikans
	@version 1.0, 28/10/2014
*/

#include "GraphicsManager.h"


/* OpenGL libs */
#include <glload/gl_load.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/* Other */
#include <iostream>
#include "glfw_wrap.h"
#include "shaderManager.h"

GLuint positionBufferObject, positionBufferObjectPyramid, colourObject;
GLuint program;
GLuint vao;

GLfloat light_x, light_y, light_z, vx, vy, vz, wingAngle, wingAngle_inc, head_angle;

GLuint colourmode;
GLuint emitmode;
GLuint drawmode;
GLuint wingCount;


/* Uniforms*/
GLuint modelID, viewID, projectionID, normalMatrixID, lightPosID, colourmodeID, emitmodeID, textureID, textureModeID, specularModeID;
GLuint colorModeID;


Sphere *lightSourceModel;
Windmill *windmill;

GLfloat aspect_ratio;

GLfloat zoom;
int window_w, window_h;

/* Pre-define functions, so that the constructor could be defined at the top of the file. */
static void keyCallback(GLFWwindow* window, int k, int s, int action, int mods);
static void resizeWindow(GLFWwindow* window, int w, int h);
static void display();
static void errorCallback(int error, const char* description);

/* Constructor for this object. */
GraphicsManager::GraphicsManager()
{
	window_w = 1024;
	window_h = 500;

	wingCount = 5;

	Glfw_wrap *glfw = new Glfw_wrap(window_w, window_h, "Assignement 1 prep, JS");

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
	}
	else
	{
		/* Note it you might want to move this call to the wrapper class */
		glfw->setErrorCallback(errorCallback);
		glfw->setRenderer(display);
		glfw->setKeyCallback(keyCallback);
		glfw->setReshapeCallback(resizeWindow);

		this->init(glfw);

		glfw->eventLoop();

		delete(glfw);
	}
}

/* De-constructor for this object. */
GraphicsManager::~GraphicsManager()
{
}

/* Initialization method - could be moved inside the constructor. */
void GraphicsManager::init(Glfw_wrap *glfw)
{
	/* General GLFW config. */
	aspect_ratio = window_w / window_h;

	/* Scene variables.*/
	zoom	   = 5.0f;
	colourmode = 0;
	emitmode   = 0;

	light_x = 0.7; 
	light_y = 0.7;
	light_z = 1.0;

	vx = 0; 
	vx = 0;
	vz = 0.f;

	wingAngle = 0.0f;
	wingAngle_inc = 0.1f;
	head_angle = 0.0f;

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	/* Create the light source object. */
	lightSourceModel = new Sphere(0.5, 0.5, false, textureID);
	lightSourceModel->makeVBO(20.0f, 30.0f);
	
	/* Create shader manager and load the shader programs. */
	ShaderManager *shaderManager = new ShaderManager();

	try
	{
		program = shaderManager->LoadShader("shaders/data.vert", "shaders/data.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to vertex shader */
	normalMatrixID = glGetUniformLocation(program, "normalmatrix");
	modelID		   = glGetUniformLocation(program, "model");
	viewID		   = glGetUniformLocation(program, "view");
	projectionID   = glGetUniformLocation(program, "projection");
	lightPosID	   = glGetUniformLocation(program, "lightPos");
	colourmodeID   = glGetUniformLocation(program, "colourmode");
	emitmodeID	   = glGetUniformLocation(program, "emitmode");
	textureID	   = glGetUniformLocation(program, "tex1");
	textureModeID  = glGetUniformLocation(program, "textureMode");
	specularModeID = glGetUniformLocation(program, "specularMode");

	/* Create a windmill object/ */
	windmill = new Windmill(wingCount, 4.0, 1.0, 0.73, 1.1, modelID, normalMatrixID, textureID, textureModeID, specularModeID);
}

/* The display callback method which redraws the scene. */
void display()
{
	/* Set the angle for wings */
	windmill->setWingAngle(wingAngle);
	wingAngle += wingAngle_inc;

	if (wingAngle > 360.0f || wingAngle < -360.0f)
	{
		wingAngle = std::fmod(wingAngle, 360.0f);
	}

	/* Anti aliasing */
	glEnable(GL_MULTISAMPLE);

	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program);

	// Projection matrix.
	glm::mat4 Projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);

	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, zoom),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
		);

	// Apply rotations to the view position
	//View = glm::scale(View, glm::vec3(zoom, zoom, zoom));
	View = glm::rotate(View, -vx, glm::vec3(1, 0, 0));
	View = glm::rotate(View, -vy, glm::vec3(0, 1, 0));
	View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));

	/* Set the light position. */
	glm::vec4 lightPos = View * glm::vec4(light_x, light_y, light_z, 1.0);

	/* Define a stack objects for model transformations. */
	glm::mat4 model = glm::mat4(1.0f);
	std::stack<glm::mat4> modelTranslate;
	std::stack<glm::mat4> modelScale;
	std::stack<glm::mat4> modelRotate;

	modelTranslate.push(glm::mat4(1.0f));
	modelScale.push(glm::mat4(1.0f));
	modelRotate.push(glm::mat4(1.0f));

	glm::mat3 gl_NormalMatrix = glm::mat3(1.0f);

	/* Individual Objects */

		/* START Windmill */
			/* THE WORST MISTAKE...
				Instead of

				glUniform4fv(lightPosID, 1, glm::value_ptr(lightPos));

				I had

				glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
			*/

			model		    = modelTranslate.top() * modelScale.top() * modelRotate.top();
			gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

			// Send uniform variables to the shader,
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
			glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
			glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
			glUniform4fv(lightPosID, 1, glm::value_ptr(lightPos));
			glUniform1ui(colourmodeID, colourmode);
			glUniform1ui(emitmodeID, emitmode);
			glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);


			/* Draw our Windmill */
			modelScale.push(glm::scale(modelScale.top(), glm::vec3(0.4, 0.4, 0.4)));
			windmill->draw(View, modelTranslate, modelScale, modelRotate);
			modelScale.pop();
		/* END Windmill */


		/* START LIGHT Sphere */
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(light_x, light_y, light_z));
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			model = glm::rotate(model, -lightSourceModel->angle_x, glm::vec3(1, 0, 0));
			model = glm::rotate(model, -lightSourceModel->angle_y, glm::vec3(0, 1, 0));
			model = glm::rotate(model, -lightSourceModel->angle_z, glm::vec3(0, 0, 1));

			gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));

			/* Send those uniforms which have changed. */
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
			glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);

			/* Draw light object */
			emitmode = 1;
			glUniform1ui(emitmodeID, emitmode);
			lightSourceModel->draw();
			emitmode = 0;

		/* End LIGHT Sphere */

	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

/* Window resize callback. */
static void resizeWindow(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);
}

/* An error callback function to output GLFW errors*/
static void errorCallback(int error, const char* description)
{
	fputs(description, stderr);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int k, int s, int action, int mods)
{
	if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	/* Valid input START*/
	if (k == 'K') wingAngle_inc += 0.05f;
	if (k == 'L') wingAngle_inc -= 0.05f;


	if (k == 'O' || k == 'P')
	{
		/* Set the angle for windmill head */

		if (k == 'O')
		{
			head_angle += 2;
		}
		else
		{
			head_angle -= 2;
		}

		if (head_angle > 360.0f || head_angle < -360.0f)
		{
			head_angle = std::fmod(head_angle, 360.0);
		}

		windmill->setHeadAngle(head_angle);
	}

	/* Cycle between drawing vertices, mesh and filled polygons */
	if (k == 'N' && action != GLFW_PRESS)
	{
		drawmode++;
		if (drawmode > 2) drawmode = 0;

		lightSourceModel->setDrawmode(drawmode);
		windmill->setDrawmode(drawmode);
	}


	if (k == '1') light_x -= 0.05f;
	if (k == '2') light_x += 0.05f;
	if (k == '3') light_y += 0.05f;
	if (k == '4') light_y -= 0.05f;
	if (k == '5') light_z -= 0.05f;

	if (k == '6') light_z += 0.05f;
	/* Valid input END */

	
	if (k == 'Z') zoom -= 0.07f;
	if (k == 'X') zoom += 0.07f;


	if (k == '7') vx -= 1.f;
	if (k == '8') vx += 1.f;
	if (k == '9') vy -= 1.f;
	if (k == '0') vy += 1.f;
	if (k == 'U') vz -= 1.f;
	if (k == 'I') vz += 1.f;

	if (k == 'M' && action != GLFW_PRESS)
	{
		colourmode = !colourmode;
		std::cout << "colourmode=" << colourmode << std::endl;
	}
}

void GraphicsManager::cmdManager()
{
	std::cout << "J.Stikans, Dundee" << std::endl;

	std::cout << "\n\n-----------------General-----------------------------------" << std::endl;
	std::cout << "\tZ - zoom in." << std::endl;
	std::cout << "\tX - zoom out.\n" << std::endl;
	std::cout << "\tK - Increase counterclockwise roatation speed for the windmill sails." << std::endl;
	std::cout << "\tL - Increase clockwise roatation speed for the windmill sails.\n" << std::endl;
	std::cout << "\tO - Rotate the windmill head (roof and sails) counterclockwise." << std::endl;
	std::cout << "\tP - Rotate the windmill head (roof and sails) clockwise." << std::endl;
	std::cout << "\n\n-----------------Lighting---------------------------------" << std::endl;
	std::cout << "\t1 - Move light source to the left on X axis." << std::endl;
	std::cout << "\t2 - Move light source to the right on X axis." << std::endl;
	std::cout << "\t3 - Move light source up on Y axis." << std::endl;
	std::cout << "\t4 - Move light source downd on Y axis." << std::endl;
	std::cout << "\t5 - Move light source further away on Z axis." << std::endl;
	std::cout << "\t6 - Move light source closer on Z axis" << std::endl;
	std::cout << "\n\n-----------------Other------------------------------------" << std::endl;
	std::cout << "\tN - Toggle between draw modes." << std::endl;
}
