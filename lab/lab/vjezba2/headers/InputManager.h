#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#include "Transform.h"
#include "Camera.h"
#include "Object.h"
#include "Lines.h"

class InputManager 
{
private:
	GLFWwindow* window;

public:
	static int WIDTH;
	static int HEIGHT;
	static int MOVE_CAMERA;

	InputManager(GLFWwindow* window);
	void InputManager::register_movable(Transform* object);
	void addLine(Lines* newLine);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(double xpos, double ypos);
	static void mouse_scroll_callback(double xoffset, double yoffset);
	static void mouse_button_callback(int button, int action, int mods);
	static void key_callback(int key, int action);
};
