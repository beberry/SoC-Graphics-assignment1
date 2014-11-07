#pragma once

/* OpenGL libs */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

#include "SOIL.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	GLuint TextureManager::load_texture(char *filename);
};

