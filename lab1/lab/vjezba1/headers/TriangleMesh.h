#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "Renderable.h"

class TriangleMesh : public Renderable
{
private:
	glm::vec3* vertices;
	glm::vec3* normals;
	glm::vec3* uvCoords;
	unsigned int* indices;
	GLuint VBO[3];
	GLuint EBO;

	int vertexCount;
	int indexCount;

	void applyTransform(glm::mat4 mat);
	void updateVertexBuffer(int i, glm::vec3* vertices, size_t size);
	std::pair<glm::vec3, glm::vec3> getBoundingBox();

public:
	TriangleMesh();
	~TriangleMesh();

	void setVertices(glm::vec3* vertices, int size);
	void setNormals(glm::vec3* normals, int size);
	void setUVCoords(glm::vec3* uvCoords, int size);
	void setIndices(unsigned int* indices, int size);
	glm::vec3* getVertices();
	glm::vec3* getNormals();
	glm::vec3* getUVCoords();
	unsigned int* getIndices();
	int getVBO(int index);
	int getEBO();

	void normalize();
	void draw() override;
};