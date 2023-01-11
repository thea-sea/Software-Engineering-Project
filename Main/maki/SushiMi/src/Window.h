#pragma once

#include <iostream>
#include <glew/include/GL/glew.h>
#include <glfw/include/GLFW/glfw3.h>

/*forward declaration*/
class Player;
class Sprite;




extern Player* player;

class Window
{
public:
	Window(int width, int height);

	~Window();

	void Input();

	void Resize();

	void Mainloop();


	//Player* player;
	Sprite* sp;
	inline static GLFWwindow* window_ptr; //moved from private to public for access in main.cpp
	
private:
	int m_width, m_height;
	GLFWwindow* ptr_win;
	double starttime, endtime, delta;
};

class pseudomain
{
public:
	static void draw();
	static void update();
	static void init();
	static void cleanup();

};