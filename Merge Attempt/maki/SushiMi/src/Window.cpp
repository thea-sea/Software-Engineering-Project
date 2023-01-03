//******************************************************************************/
/*!
\file		Window.cpp
\author 	Thea Sea
\par    	email: thea.sea@digipen.edu
\date   	2/8/2022
\brief		This source file contains the main function to call all other functions and implement the game loop

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/


/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Window.h"
#include "../../imgui/imgui.h"

Window::Window(int width, int height)
	:m_width(width),
	m_height(height)

{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, true);

	window_ptr = glfwCreateWindow(width, height, "ANIMATION ONLY", NULL, NULL);
	if (window_ptr == nullptr)
	{
		std::cout << "erorr initilize glfw" << std::endl;
		return;
	}

	glfwMakeContextCurrent(window_ptr);

	if (glewInit())
	{
		std::cout << "erorr initilize glew" << std::endl;
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	starttime = 0;
	endtime = 0;
	delta = 0;

	Shaders = std::make_unique<ShaderLibrary>();
	camera = std::make_unique<Camera>(0, 0);

	player = new Player();

	sp = new Sprite("../textures/menu.jpg");
	sp->transformation.scale = glm::vec2(2000, 2000);
	sp->transformation.position = glm::vec2(0);
}

Window::~Window()
{
	delete player;
	glfwTerminate();
}

void Window::Input()
{
	if (glfwGetKey(window_ptr, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window_ptr, true);
	}
	if (ImGui::IsKeyDown(GLFW_KEY_RIGHT))
	//if (glfwGetKey(window_ptr, GLFW_KEY_RIGHT))
	{
		//check when key is pressed only and not held to do grid snapping
		
		if (ImGui::IsKeyReleased(GLFW_KEY_RIGHT))
		{
			player->stop();
		}
		player->move_right();
	}
	else if (glfwGetKey(window_ptr, GLFW_KEY_LEFT))
	{
		player->move_left();
	}
	else if (glfwGetKey(window_ptr, GLFW_KEY_UP))
	{
		player->move_up();
	}
	else if (glfwGetKey(window_ptr, GLFW_KEY_DOWN))
	{
		player->move_down();
	}
	else
	{
		player->stop();
	}

}

void Window::Resize()
{
	int width, height;
	glfwGetWindowSize(window_ptr, &width, &height);
	if (width != m_width || height != m_height)
	{
		m_width = width;
		m_height = height;
		glViewport(0, 0, width, height);
		camera->Update_Viewport(width, height);
		printf("resized \n");
	}
}

void Window::Mainloop()
{
	while (!glfwWindowShouldClose(window_ptr))
	{
		starttime = glfwGetTime();
		pseudomain::update();

		//for each frame 
		Resize();
		Input();
		pseudomain::draw(); //swap buffers and glfwpollevents are already done here, do not call again below
		glClearColor(0.39f, 0.58f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// all drawing goes here ..
		Shaders->Textured_Shader()->use();
		Shaders->Textured_Shader()->Send_Mat4("projection", camera->Get_Projection());

		Shaders->Textured_Shader()->Send_Mat4("model_matrx", sp->transformation.Get());
		sp->draw();

		Shaders->Textured_Shader()->Send_Mat4("model_matrx", player->Transformation());
		player->draw(delta);
		

		endtime = glfwGetTime();
		delta = (endtime - starttime) / 2;
	}
	glfwSwapBuffers(window_ptr);
	glfwPollEvents();
}
