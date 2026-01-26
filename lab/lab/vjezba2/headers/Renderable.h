#pragma once

#include <glad/glad.h>

class Renderable
{
private:
	GLuint VAO;

public:
	Renderable();
	~Renderable();
	void setVAO(GLuint VAO);
	GLuint getVAO();

	virtual void draw();
};