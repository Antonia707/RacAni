#include "Renderable.h"
#include <glad/glad.h>

Renderable::Renderable() : VAO(0), color(1.0f, 1.0f, 1.0f) {}

Renderable::~Renderable() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
}

void Renderable::setVAO(GLuint vao) {
    VAO = vao;
}

GLuint Renderable::getVAO() {
    return VAO;
}

void Renderable::setColor(const glm::vec3& c) {
    color = c;
}

glm::vec3 Renderable::getColor() {
    return color;
}

void Renderable::draw() {
    if (VAO) {
        glBindVertexArray(VAO);
    }
}
