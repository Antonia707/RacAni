#include "Lines.h"
#include <iostream>
#include <glad/glad.h>

Lines::Lines(const std::vector<glm::vec3>& pts, glm::vec3 col)
    : vertices(pts), color(col) {
    VBO = 0;
    EBO = 0;

    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    setVAO(vaoID);

    if (VBO == 0)
        glGenBuffers(1, &VBO);
}

Lines::~Lines() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Lines::setVertices(std::vector<glm::vec3> v, int size) {
    vertices = v;

    if (VBO == 0)
        glGenBuffers(1, &VBO);

    updateVertexBuffer();
}

std::vector<glm::vec3> Lines::getVertices() {
    return vertices;
}

int Lines::getVBO(int index) {
    return VBO;
}

int Lines::getEBO() {
    return EBO;
}

void Lines::updateVertexBuffer() {
    glBindVertexArray(getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Lines::addPoint(glm::vec3 newPoint) {
    vertices.push_back(newPoint);
    updateVertexBuffer();
}

void Lines::draw() {
    if (vertices.empty() || getVAO() == 0) return;

    glBindVertexArray(getVAO());
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
    glBindVertexArray(0);
}

void Lines::printPoints() {
    std::cout << "Line points:\n";
    for (const auto& pt : vertices) {
        std::cout << "(" << pt.x << ", " << pt.y << ", " << pt.z << ")\n";
    }
}
