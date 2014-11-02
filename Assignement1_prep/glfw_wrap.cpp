/**
Some methods have been borrowed from I.Martins lab examples.

Modified from the OpenGL GLFW example to provide a wrapper GLFW class
and to include shader loader functions to include shaders as text files
Iain Martin August 2014
*/



/* Inlcude GL_Load and GLFW */
#include <glload/gl_load.h>
#include "glfw_wrap.h"

/* Inlcude some standard headers */
#include <iostream>



/*
	Constructor for wrapper object 
	Original code taken from Ian Martnis lab example.
*/
Glfw_wrap::Glfw_wrap(int width, int height, char *title) {

	this->windowWidth = width;
	this->windowHeight = height;
	this->title = title;
	this->fps = 60;
	this->running = true;

	/* Initialise GLFW and exit if it fails */
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW." << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	window = glfwCreateWindow(width, height, title, 0, 0);
	if (!window){
		std::cout << "Could not open GLFW window." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Obtain an OpenGL context and assign to the just opened GLFW window */
	glfwMakeContextCurrent(window);

	/* Initialise GLLoad library. You must have obtained a current OpenGL */
	if (!ogl_LoadFunctions())
	{
		std::cerr << "oglLoadFunctions() failed. Exiting" << std::endl;
		glfwTerminate();
		return;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
}


/* 
	Terminate GLFW on destruvtion of the wrapepr object
	Original code taken from Ian Martnis lab example.
*/
Glfw_wrap::~Glfw_wrap() {
	glfwTerminate();
}

/*
	Returns the GLFW window handle, required to call GLFW functions outside this class
	Original code taken from Ian Martnis lab example.
*/
GLFWwindow* Glfw_wrap::getWindow()
{
	return window;
}


/*
	GLFW_Main function normally starts the windows system, calls any init routines
	and then starts the event loop which runs until the program ends

	Original code taken from Ian Martnis lab example.
*/
int Glfw_wrap::eventLoop()
{
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Call function to draw your graphics
		renderer();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


/*
	Register an error callback function
	Original code taken from Ian Martnis lab example.
*/
void Glfw_wrap::setErrorCallback(void(*func)(int error, const char* description))
{
	this->errorCallback = func;
	glfwSetErrorCallback(func);
}

/* 
	Register a display function that renders in the window
	Original code taken from Ian Martnis lab example.
*/
void Glfw_wrap::setRenderer(void(*func)()) {
	this->renderer = func;
}

/* 
	Register a callback that runs after the window gets resized
	Original code taken from Ian Martnis lab example.
*/
void Glfw_wrap::setReshapeCallback(void(*func)(GLFWwindow* window, int w, int h)) {
	reshape = func;
	glfwSetFramebufferSizeCallback(window, reshape);
}


/*
	Register a callback to respond to keyboard event
	Original code taken from Ian Martnis lab example.
*/
void Glfw_wrap::setKeyCallback(void(*func)(GLFWwindow* window, int key, int scancode, int action, int mods))
{
	keyCallBack = func;
	glfwSetKeyCallback(window, keyCallBack);
}