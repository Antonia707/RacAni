#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Lines.h"

#include "Renderable.h"

class Lines : public Renderable {
private:
	std::vector<glm::vec3> vertices;
	GLuint VBO, EBO;

	void updateVertexBuffer();

public:
	Lines(const std::vector<glm::vec3>& points, glm::vec3 color = glm::vec3(0.7f));
	~Lines();

	void setVertices(std::vector<glm::vec3> v, int size);
	std::vector<glm::vec3> getVertices();
	int getVBO(int index);
	int getEBO();

	void addPoint(glm::vec3 newPoint);
	void printPoints();
	void draw() override;
};
