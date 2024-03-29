#pragma once
/* !
@file		fonts.h
@author		louishetong.wang@digipen.edu
@date		5/10/2022

This file implements functionality for fonts using freetype for showing words on screen

*//*__________________________________________________________________________*/

#include <glm/glm/glm.hpp>
#include <map>
#include "../Engine/System/Graphics/glslshader.h"

#ifndef FONT_H_
#define FONT_H_

class Font
{
public:
	struct Character
	{
		unsigned int	TextureID;			// ID handle of the glyph texture
		glm::ivec2		Size;				// Size of glyph
		glm::ivec2		Bearing;			// Offset from baseline to left/top of glyph
		unsigned int	Advance;			// Offset to advance to next glyph
	};
	static std::map<char, Character> Characters;	// Singleton
	static int init();
	static void setup_shdrpgm();
	static void RenderText(GLSLShader& s, std::string text, float x, float y, float scale, glm::vec3 color);

};

extern unsigned int VAO, VBO;

#endif // !FONT_H_
