#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <limits>
#include <cstring>

#include "TriangleMesh.h"
#include <algorithm>

TriangleMesh::TriangleMesh()
    : vertices(nullptr), normals(nullptr), uvCoords(nullptr), indices(nullptr),
    vertexCount(0), indexCount(0) {
    std::fill(VBO, VBO + 3, 0);
    EBO = 0;

    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    setVAO(vaoID);
}

TriangleMesh::~TriangleMesh() {
    delete[] vertices;
    delete[] normals;
    delete[] uvCoords;
    delete[] indices;

    glDeleteBuffers(3, VBO);
    glDeleteBuffers(1, &EBO);
}

void TriangleMesh::updateVertexBuffer(int i, glm::vec3* vertices, size_t size) {
    glBindVertexArray(getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(i);
    glBindVertexArray(0);
}

void TriangleMesh::setVertices(glm::vec3* verts, int size) {
    delete[] vertices;
    vertexCount = size;
    vertices = new glm::vec3[size];
    memcpy(vertices, verts, size * sizeof(glm::vec3));

    if (VBO[0] == 0)
        glGenBuffers(1, &VBO[0]);

	updateVertexBuffer(0, vertices, size);
}

void TriangleMesh::setNormals(glm::vec3* norms, int size) {
    normals = new glm::vec3[size];
    memcpy(normals, norms, size * sizeof(glm::vec3));

    if (VBO[1] == 0)
        glGenBuffers(1, &VBO[1]);

    updateVertexBuffer(1, normals, size);
}

void TriangleMesh::setUVCoords(glm::vec3* uvs, int size) {
    uvCoords = new glm::vec3[size];
    memcpy(uvCoords, uvs, size * sizeof(glm::vec3));

    if (VBO[2] == 0)
        glGenBuffers(1, &VBO[2]);

    updateVertexBuffer(2, uvCoords, size);
}

void TriangleMesh::setIndices(unsigned int* inds, int size) {
    indexCount = size;
    indices = new unsigned int[size];
    memcpy(indices, inds, size * sizeof(unsigned int));

    if (EBO == 0)
        glGenBuffers(1, &EBO);

    glBindVertexArray(getVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

glm::vec3* TriangleMesh::getVertices() { return vertices; }
glm::vec3* TriangleMesh::getNormals() { return normals; }
glm::vec3* TriangleMesh::getUVCoords() { return uvCoords; }
unsigned int* TriangleMesh::getIndices() { return indices; }

int TriangleMesh::getVBO(int index) { return VBO[index]; }
int TriangleMesh::getEBO() { return EBO; }

void TriangleMesh::applyTransform(glm::mat4 mat) {
    for (int i = 0; i < vertexCount; ++i) {
        glm::vec4 v = glm::vec4(vertices[i], 1.0f);
        vertices[i] = glm::vec3(mat * v);
    }

    if (normals) {
        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(mat)));
        for (int i = 0; i < vertexCount; ++i) {
            normals[i] = glm::normalize(normalMat * normals[i]);
        }
    }

	updateVertexBuffer(0, vertices, vertexCount);
	if (normals)
	    updateVertexBuffer(1, normals, vertexCount);
}

std::pair<glm::vec3, glm::vec3> TriangleMesh::getBoundingBox() {
    glm::vec3 min(FLT_MAX), max(-FLT_MAX);
    for (int i = 0; i < vertexCount; ++i) {
        min = glm::min(min, vertices[i]);
        max = glm::max(max, vertices[i]);
    }
    return { min, max };
}

void TriangleMesh::normalize() {
    if (!vertices || vertexCount == 0) return;

    std::pair<glm::vec3, glm::vec3> boundingBox = getBoundingBox();
    glm::vec3 min = boundingBox.first;
    glm::vec3 max = boundingBox.second;

    glm::vec3 center = (min + max) * 0.5f;
    float maxExtent = glm::compMax(max - min);
    float scale = 2.0f / maxExtent;

    glm::mat4 normalizationMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
    normalizationMatrix = glm::translate(normalizationMatrix, -center);

    applyTransform(normalizationMatrix);
}

void TriangleMesh::draw() {
    if (indexCount == 0 || getVAO() == 0) return;

    glBindVertexArray(getVAO());
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
