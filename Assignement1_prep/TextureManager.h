/**
	Texture loader. The origianal code was provided by Ian Martin. Extracted into
	a class	and modified by me.

	@author Ian Martin, Modified by Jekabs Stikanss
	@version 1.0, 07/11/2014
*/

#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

#include "SOIL.h"
#include <iostream>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	GLuint TextureManager::load_texture(char *filename);
};