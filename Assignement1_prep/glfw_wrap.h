#pragma once

/* Inlcude GL_Load and GLFW */
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>
#include "glfw_wrap.h"

/* Inlcude some standard headers */
#include <iostream>

class Glfw_wrap
{
private:
	bool running;
	GLFWwindow* window;

	int windowWidth;
	int windowHeight;

	char *title;

	double fps;

	void(*errorCallback)(int error, const char* description);
	void(*renderer)();
	void(*resizeWindow)(GLFWwindow* window, int w, int h);
	void(*keyCallBack)(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	Glfw_wrap(int width, int height, char *title);
	~Glfw_wrap();

	void setFPS(double fps) {
		this->fps = fps;
	}

	/* Callback registering functions */
	void setRenderer(void(*f)());
	void setReshapeCallback(void(*f)(GLFWwindow* window, int w, int h));
	void setKeyCallback(void(*f)(GLFWwindow* window, int key, int scancode, int action, int mods));
	void setErrorCallback(void(*f)(int error, const char* description));


	int eventLoop();
	GLFWwindow* getWindow();
};