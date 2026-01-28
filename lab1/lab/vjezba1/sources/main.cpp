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
#include "BSpline.h"

int InputManager::WIDTH = 900;
int InputManager::HEIGHT = 900;

int main(int argc, char* argv[]) {
    std::string path(argv[0]);
    std::string dirPath(path, 0, path.find_last_of("\\/"));
    std::string resPath(dirPath);
    resPath.append("\\resources");
    std::string objPath(resPath);
    objPath.append("\\airplane\\airplane.obj");
    std::string splinePath(resPath);
    splinePath.append("\\spline\\spline.obj");

    if (!glfwInit()) {
        std::cerr << "GLFW Initialization failed!" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(InputManager::WIDTH, InputManager::HEIGHT, "Vjezba 1", nullptr, nullptr);
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

    Shader* shader2 = Shader::loadShader(argv[0], "shader2");
    if (!shader2) {
        std::cerr << "Failed to load shader2!" << std::endl;
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

    Renderer renderer = Renderer(glm::vec2(InputManager::WIDTH, InputManager::HEIGHT));
    renderer.setDepthShader(shader1);
    renderer.setDepthMap(new DepthTexture());
    InputManager inputManager = InputManager(window);

    // kamera
    inputManager.register_movable(&(renderer.getCamera()));
    renderer.getCamera().globalMove(glm::vec3(0, 0, -10));
    renderer.getCamera().setOrientation(glm::vec3(0, 0, 1), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1));

	// svjetlo
    Light* light = new Light();
    light->globalMove(glm::vec3(0, 0, -10));
    light->setOrientation(glm::vec3(0, 0, 1), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1));
	light->setInnerCutoff(glm::cos(glm::radians(50.0f)));
	light->setOuterCutoff(glm::cos(glm::radians(55.5f)));

	// objekti
    Object* obj = new Object(shader2, nullptr);
    if (!ResourceManager::getScene(objPath, *obj)) {
        std::cerr << "Failed to load model!" << std::endl;
        delete obj;
    }
    renderer.registerObject(obj);
    inputManager.register_movable(obj);

	// linije
    Lines* line = new Lines(std::vector<glm::vec3>());
    if (!ResourceManager::getVertices(splinePath, *line)) {
        std::cerr << "Failed to load vertices!" << std::endl;
    }
    Object controlPolygon(shader, line);
    //renderer.registerObject(&controlPolygon);

    BSpline* spline = new BSpline(100, line->getVertices());
	spline->setColor(glm::vec3(0.0f, 0.0f, 0.0f));
    Object bSpline(shader, spline);
    renderer.registerObject(&bSpline);

    //tangente
	std::vector<std::vector<glm::vec3>> tangents = spline->getTangentLines(50, 3);
	for (size_t i = 0; i < tangents.size(); i++) {
		Lines* tangentLine = new Lines(tangents[i]);
        tangentLine->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		Object* tangentObj = new Object(shader, tangentLine);
		renderer.registerObject(tangentObj);
	}

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    InputManager::MOVE_OBJECT = 0;
    static int i = 0;
    static float t = 0.0f;
    float dt = 0.10f;
    while (!glfwWindowShouldClose(window)) {
        if (InputManager::MOVE_OBJECT && spline->getVertices().size() > 1) {
            const auto& vertices = spline->getVertices();
            int n = vertices.size();

            obj->setPosition(glm::mix(vertices[i], vertices[(i + 1) % n], t));
			BSpline::Orientation orientation = spline->getOrientation(vertices[i]);
            obj->setOrientation(-orientation.forward, orientation.up, orientation.right);

            t += dt;
            if (t >= 1.0f) {
                t = 0.0f;
                i = (i + 1) % (n - 1);
            }
        }

        glfwPollEvents();

        glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// prvi prolaz
		renderer.depthRender(*light);
		// drugi prolaz
		renderer.render(*light);

		glfwSwapBuffers(window);
    }

    delete shader1;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
