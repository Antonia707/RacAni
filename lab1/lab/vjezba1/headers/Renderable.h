#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderable
{
private:
	GLuint VAO;
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

public:
	Renderable();
	~Renderable();
	void setVAO(GLuint VAO);
	GLuint getVAO();
	void setColor(const glm::vec3& c);
	glm::vec3 getColor();

	virtual void draw();
};