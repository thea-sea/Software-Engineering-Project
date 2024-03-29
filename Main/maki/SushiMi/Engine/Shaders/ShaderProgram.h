#pragma once
/*!
@file		ShaderProgram.h
@author		louishetong.wang@digipen.edu
@date		20/11/2022

@brief		Redesigned Shader program that compiles the shaders, create them and
			attach them to the shader programs using their IDs. SendMat4 will
			set uniform location for their uniform variables(model_matrx or projection).
*//*__________________________________________________________________________*/
#include <string>
#include <iostream>
//using namespace std;
#include "../../glm/glm/mat4x4.hpp"
#include "../../glew/include/GL/glew.h"

class ShaderProgram
{
public:
	ShaderProgram(std::string vertex_shader, std::string fragment_shader);

	~ShaderProgram();

	void use();

	void Send_Mat4(const char* name, glm::mat4 mat);

	void Send_Alpha(const char* name, float alpha);

	GLuint get_hdl();

private:
	void checkerorr(unsigned int shader_id, std::string type);
	 GLuint ID;
};