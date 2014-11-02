#pragma once

#include <string>

#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

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

