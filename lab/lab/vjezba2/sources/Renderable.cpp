#include "Renderable.h"
#include <glad/glad.h>

Renderable::Renderable() : VAO(0) {}

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

void Renderable::draw() {
    if (VAO) {
        glBindVertexArray(VAO);
    }
}
