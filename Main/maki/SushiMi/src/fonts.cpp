
/*                                                                   includes
----------------------------------------------------------------------------- */
#include <iostream>
#include "include/common_headers.hpp"
#include "include/glapp.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "fonts.h"

/*--------------------------------------------------------------------------- */


/*  _________________________________________________________________________ */

unsigned int VAO, VBO;

std::map<char, Font::Character> Font::Characters{};
int Font::init()
{
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLApp::shdrpgms["shdrpgm"].Use();
	FT_Library ft;	//init the freetype library ft
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR: Unable to init freetype library." << std::endl;
		return -1;
	}
	FT_Face face;	//load font as a face
    
	if (FT_New_Face(ft, "../amiri-regular.ttf", 0, &face))	//try to load arial font here
	{
		std::cout << "ERROR: Arial not found!" << std::endl;
		return -1;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		std::cout << "ERROR:FREETYPE: Failed to load Glyph" << std::endl;
		return -1;
	}

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    Font::setup_shdrpgm();

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return 1;
    GLApp::shdrpgms["shdrpgm"].UnUse();
}

void Font::setup_shdrpgm()
{
    std::string vs_source =
        "#version 450 core                             \n"
        "layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>;  \n"
        "out vec2 TexCoords;                           \n"
        "uniform mat4 projection;           		   \n"
        ""
        "void main(void)                               \n"
        "{                                             \n"
        "gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
        "TexCoords = vertex.zw;        				   \n"
        "}                                             \n";

    std::string fs_source =
        "#version 450 core                             \n"
        "in vec2 TexCoords;                            \n"
        "out vec4 color;                               \n"
        "uniform sampler2D text;                       \n"
        "uniform vec3 textColor;                       \n"    
        "void main(void)                               \n"
        "{                                             \n"
        "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);       \n"
        "color = vec4(textColor, 1.0) * sampled;       \n"   
        "}                                             \n";
    GLApp::shdrpgms["shdrpgm"].CompileShaderFromString(GL_VERTEX_SHADER, vs_source);
    GLApp::shdrpgms["shdrpgm"].CompileShaderFromString(GL_FRAGMENT_SHADER, fs_source);
    GLApp::shdrpgms["shdrpgm"].Link();
    GLApp::shdrpgms["shdrpgm"].Validate();

    if (!GLApp::shdrpgms["shdrpgm"].IsLinked()) {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << GLApp::shdrpgms["shdrpgm"].GetLog() << "\n";
        std::exit(EXIT_FAILURE);
    }
}

void Font::RenderText(GLSLShader& s, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    s.Use();
    glUniform3f(glGetUniformLocation(s.GetHandle(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

