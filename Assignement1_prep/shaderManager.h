/**
	Shader program loader. The origianal code was provided by Ian Martin. Extracted into
	a class	and modified by me.

	@author Ian Martin, Modified by Jekabs Stikanss
	@version 1.0, 29/10/2014
*/
#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

/* Some STL includes. */
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class ShaderManager
{
private:

public:
	ShaderManager();
	~ShaderManager();

	/* Shader load and build support functions */
	GLuint LoadShader(const char *vertex_path, const char *fragment_path);
	GLuint BuildShader(GLenum eShaderType, const std::string &shaderText);
	GLuint BuildShaderProgram(std::string vertShaderStr, std::string fragShaderStr);
	std::string readFile(const char *filePath);
};

