/**
	An object which is responsible for managing the whole application - draws the frames,
	reacts to callbacks.

	@author Jekabs Stikans
	@version 1.0, 28/10/2014
*/
#pragma once

/* Inlcude GL_Load and GLFW */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/* OpenGL objects. */
#include "Sphere.h"
#include "Windmill.h"

/* Other */
#include <stack>
#include <iostream>
#include "glfw_wrap.h"
#include "shaderManager.h"

class GraphicsManager
{
private:
	void GraphicsManager::init(Glfw_wrap *glw);
	void GraphicsManager::cmdManager();

public:
	GraphicsManager();
	~GraphicsManager();
};

