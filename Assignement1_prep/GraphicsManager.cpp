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
GLuint numspherevertices;
GLuint drawmode;			// Defines drawing mode of sphere as points, lines or filled polygons

GLfloat light_x, light_y, light_z, vx, vy, vz;;

GLuint colourmode;	/* Index of a uniform to switch the colour mode in the vertex shader
					I've included this to show you how to pass in an unsigned integer into
					your vertex shader. */
GLuint emitmode;



/* Uniforms*/
GLuint modelID, viewID, projectionID, normalMatrixID, lightPosID, colourmodeID, emitmodeID;
GLuint colorModeID;

Cylinder *wmbom;
GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/


void GraphicsManager::init(Glfw_wrap *glfw)
{
	aspect_ratio = 1.3333f;
	colourmode = 0; emitmode = 0;

	light_x = 0; light_y = 0; light_z = 0;
		vx = 0; vx = 0, vz = 0;

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	wmbom = new Cylinder(8.0);

	numspherevertices = wmbom->makeVBO(2.0f, 30.0f);
	
	/* Create a vertex buffer object to store vertex colours */
	//glGenBuffers(1, &colourObject);
	//glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	//glBufferData(GL_ARRAY_BUFFER, cubeModel->getVertexColoursSize()*sizeof(GLfloat), cubeModel->getVerteColours(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	modelID = glGetUniformLocation(program, "model");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	colorModeID = glGetUniformLocation(program, "colorMode");
	lightPosID = glGetUniformLocation(program, "lightPos");
	colourmodeID = glGetUniformLocation(program, "colourmode");
	emitmodeID = glGetUniformLocation(program, "emitmode");
}


void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program);

	/* Bind cube vertices. Note that this is in attribute index 0 */
	//glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index 1 */
	//glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube normals. Note that this is in attribute index 2 */
	//glEnableVertexAttribArray(2);
	//glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Define the model transformations for the cube
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(x + 0.5, y, z));
	//model = glm::scale(model, glm::vec3(scale, scale, scale));//scale equally in all axis
	model = glm::rotate(model, -wmbom->angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -wmbom->angle_y, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -wmbom->angle_z, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

	glm::mat4 Projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);

	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 10), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// Apply rotations to the view position
	View = glm::rotate(View, -vx, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	View = glm::rotate(View, -vy, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));

	// Calculate normal matrix
	glm::mat3 gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View * model)));

	glm::vec4 lightPos = View * glm::vec4(light_x, light_y, light_z, 1.0);

	// Send our uniforms variables to the currently bound shader,
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	//glUniform1ui(colourmodeID, colourmode);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(lightPosID, 1, GL_FALSE, glm::value_ptr(lightPos));
	glUniform1ui(colourmodeID, colourmode);
	glUniform1ui(emitmodeID, emitmode);


	/* Draw our cube*/
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	/* Define the model transformations for our sphere */
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(-x - 0.5, 0, 0));
	//model = glm::scale(model, glm::vec3(scale / 3.f, scale / 3.f, scale / 3.f));//scale equally in all axis
	//model = glm::rotate(model, -angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	//model = glm::rotate(model, -angle_y, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	//model = glm::rotate(model, -angle_z, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

	gl_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View*model)));


	// Send our uniforms variables to the currently bound shader,
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, &gl_NormalMatrix[0][0]);


	/* Draw our sphere */
	wmbom->draw();

	glDisableVertexAttribArray(0);




	glUseProgram(0);


	wmbom->angle_x += wmbom->angle_inc_x;
	wmbom->angle_y += wmbom->angle_inc_y;
	wmbom->angle_z += wmbom->angle_inc_z;

}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
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
void keyCallback(GLFWwindow* window, int k, int s, int action, int mods)
{
	if (action != GLFW_PRESS) return;

	if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (k == 'Q') wmbom->angle_inc_x -= 0.05f;
	if (k == 'W') wmbom->angle_inc_x += 0.05f;
	if (k == 'E') wmbom->angle_inc_y -= 0.05f;
	if (k == 'R') wmbom->angle_inc_y += 0.05f;
	if (k == 'T') wmbom->angle_inc_z -= 0.05f;
	if (k == 'Y') wmbom->angle_inc_z += 0.05f;

	if (k == '1') light_x -= 0.05f;
	if (k == '2') light_x += 0.05f;
	if (k == '3') light_y -= 0.05f;
	if (k == '4') light_y += 0.05f;
	if (k == '5') light_z -= 0.05f;
	if (k == '6') light_z += 0.05f;

	if (k == '7') vx -= 1.f;
	if (k == '8') vx += 1.f;
	if (k == '9') vy -= 1.f;
	if (k == '0') vy += 1.f;
	if (k == 'O') vz -= 1.f;
	if (k == 'P') vz += 1.f;

	if (k == 'M' && action != GLFW_PRESS)
	{
		colourmode = !colourmode;
		std::cout << "colourmode=" << colourmode << std::endl;
	}

	/* Cycle between drawing vertices, mesh and filled polygons */
	if (k == 'N' && action != GLFW_PRESS)
	{
		drawmode++;
		if (drawmode > 2) drawmode = 0;

		wmbom->setDrawmode(drawmode);
	}


}


GraphicsManager::GraphicsManager()
{
	Glfw_wrap *glfw = new Glfw_wrap(1024, 500, "Assignement 1 prep");

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
		glfw->setReshapeCallback(reshape);

		this->init(glfw);

		glfw->eventLoop();

		delete(glfw);
	}
}


GraphicsManager::~GraphicsManager()
{
}