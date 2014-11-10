/**
	Texture loader. The origianal code was provided by Ian Martin. Extracted into
	a class	and modified by me.

	@author Ian Martin, Modified by Jekabs Stikanss
	@version 1.0, 07/11/2014
*/
#include "TextureManager.h"

/* Constructor for this object. */
TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

/* Texture loader. Originally written by Ian Martin.*/
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