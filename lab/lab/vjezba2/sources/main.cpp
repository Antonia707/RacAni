#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Shader.h"
#include "Object.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Renderer.h"
#include "Lines.h"
#include "BezierBuilder.h"

int InputManager::WIDTH = 900;
int InputManager::HEIGHT = 900;

int main(int argc, char* argv[]) {
    std::string path(argv[0]);
    std::string dirPath(path, 0, path.find_last_of("\\/"));
    std::string resPath(dirPath);
    resPath.append("\\resources");
    std::string objPath(resPath);
    objPath.append("\\snow\\snow.obj");

    if (!glfwInit()) {
        std::cerr << "GLFW Initialization failed!" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(InputManager::WIDTH, InputManager::HEIGHT, "Vjezba ", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
	InputManager im(window);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetFramebufferSizeCallback(window, InputManager::framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, InputManager::WIDTH, InputManager::HEIGHT);

    Shader* shader1 = Shader::loadShader(argv[0], "shader1"); 
    if (!shader1) {
        std::cerr << "Failed to load shader1!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    Shader* shader = Shader::loadShader(argv[0], "shader");
    if (!shader) {
        std::cerr << "Failed to load shader!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    Object glava1(shader1, nullptr);

    if (!ResourceManager::getScene(objPath, glava1)) {
        std::cerr << "Failed to load model!" << std::endl;
        delete shader1;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    Object snowflake(shader, nullptr);

    Light *light = new Light();
    light->globalMove(glm::vec3(0, 0, 5));
    light->setOrientation(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1));

    if (!ResourceManager::getScene(objPath, snowflake)) {
        std::cerr << "Failed to load model!" << std::endl;
        delete shader1;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

	glava1.globalMove(glm::vec3(-1, 0, 0));
	glava1.rotate(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0)));
	glava1.scaleF(glm::vec3(0.5f));

	snowflake.globalMove(glm::vec3(0.6, 0, 0));
    snowflake.setOrientation(glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));

	Renderer renderer = Renderer(glm::vec2(InputManager::WIDTH, InputManager::HEIGHT));
	renderer.registerObject(&glava1);
    renderer.registerObject(&snowflake);

	renderer.getCamera().globalMove(glm::vec3(0, 0, 5));
	renderer.getCamera().setOrientation(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1));

    InputManager inputManager = InputManager(window);

    inputManager.register_movable(&(renderer.getCamera()));
    inputManager.register_movable(&glava1);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    InputManager::MOVE_CAMERA = 0;
    int t = 0;
    int i = 1;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, InputManager::WIDTH, InputManager::HEIGHT);
		renderer.render(*light);

		glfwSwapBuffers(window);
    }

    delete shader;
    delete shader1;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
