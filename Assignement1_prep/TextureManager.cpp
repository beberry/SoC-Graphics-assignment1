#include "TextureManager.h"
#include <iostream>

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

GLuint TextureManager::load_texture(char *filename)
{
	GLuint SOIL_response = SOIL_load_OGL_texture(filename, 0, 0, SOIL_FLAG_POWER_OF_TWO);
	
	if (!SOIL_response)
	{
		printf("Could not load file: %s\n", filename);
		exit(1);
	}

	return (SOIL_response);
}
