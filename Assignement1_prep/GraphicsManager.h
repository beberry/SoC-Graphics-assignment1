#pragma once

/* Inlcude GL_Load and GLFW */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>

// OpenGL objects.
#include "Cube.h"
#include "Cylinder.h"

// Other
#include "glfw_wrap.h"

class GraphicsManager
{
private:
	void GraphicsManager::init(Glfw_wrap *glw);
	Cube *cubeModel;
	Cylinder *windmillBaseModel;

public:
	GraphicsManager();
	~GraphicsManager();

};

