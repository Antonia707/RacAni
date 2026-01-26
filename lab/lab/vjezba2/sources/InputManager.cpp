#include "InputManager.h"
#include <glm/ext/matrix_transform.hpp>

#include "BezierBuilder.h"

std::vector<Transform*> movableObjects;
std::vector<Lines*> lines;
int currentObjectIndex;
float lastX, lastY;
bool firstMouse;
float mouseSensitivity;

int InputManager::MOVE_CAMERA = 0;

InputManager::InputManager(GLFWwindow* window) : window(window) {
    currentObjectIndex = 0;
    lastX = 400.0f;
    lastY = 300.0f;
    firstMouse = false;
    mouseSensitivity = 0.2f;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, [](GLFWwindow*, double x, double y) {
        InputManager::mouse_callback(x, y);
        });

    glfwSetScrollCallback(window, [](GLFWwindow*, double x, double y) {
        InputManager::mouse_scroll_callback(x, y);
        });

    glfwSetMouseButtonCallback(window, [](GLFWwindow*, int b, int a, int m) {
        InputManager::mouse_button_callback(b, a, m);
        });

    glfwSetKeyCallback(window, [](GLFWwindow*, int key, int, int action, int) {
        InputManager::key_callback(key, action);
        });
}

void InputManager::register_movable(Transform* object)
{
    movableObjects.push_back(object);
}

void InputManager::addLine(Lines *newLine)
{
    lines.push_back(newLine);
}

void InputManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
}

void InputManager::mouse_callback(double xpos, double ypos) {
    if (movableObjects.empty()) return;
    Transform* obj = movableObjects[currentObjectIndex];

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (float)(xpos - lastX) * mouseSensitivity;
    float yoffset = (float)(lastY - ypos) * mouseSensitivity;

    lastX = xpos;
    lastY = ypos;

    glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(yoffset), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 combinedRotation = rotationMatrixX * rotationMatrixY;

    obj->rotate(combinedRotation);
}

void InputManager::mouse_scroll_callback(double xoffset, double yoffset) {
    if (movableObjects.empty()) return;
    Transform* obj = movableObjects[currentObjectIndex];

    glm::vec3 scaleDelta = glm::vec3(1.0f + yoffset * 0.1f);
    obj->scaleF(scaleDelta);
}

void InputManager::mouse_button_callback(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        lines[0]->addPoint(movableObjects[0]->getPosition());
        lines[1]->setVertices(BezierBuilder::makeApproximationCurve(100, lines[0]->getVertices()), 100);
        lines[2]->setVertices(BezierBuilder::makeInterpolationCurve(lines[0]->getVertices()), 4);
    }
}

void InputManager::key_callback(int key, int action) {
    if (movableObjects.empty() || action != GLFW_PRESS) return;

    Transform* obj = movableObjects[currentObjectIndex];
    float moveSpeed = 0.2f;

    switch (key) {
    case GLFW_KEY_W: obj->globalMove({ 0, 0, -moveSpeed }); break;
    case GLFW_KEY_S: obj->globalMove({ 0, 0,  moveSpeed }); break;
    case GLFW_KEY_A: obj->globalMove({ -moveSpeed, 0, 0 }); break;
    case GLFW_KEY_D: obj->globalMove({ moveSpeed, 0, 0 }); break;
    case GLFW_KEY_Q: obj->globalMove({ 0, -moveSpeed, 0 }); break;
    case GLFW_KEY_E: obj->globalMove({ 0, moveSpeed, 0 }); break;
    case GLFW_KEY_SPACE: MOVE_CAMERA = 1; break;
    case GLFW_KEY_TAB:
        currentObjectIndex = (currentObjectIndex + 1) % movableObjects.size();
        break;
    default: break;
    }
}