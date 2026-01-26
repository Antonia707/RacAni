#pragma once
#include <vector>
#include <iostream>

#include "Object.h"
#include "Camera.h"
#include "Light.h"

class Renderer
{
private:
	std::vector<Object*> objects;
	// Light light;
	Camera camera;

	glm::vec2 screenSize;

public:
	Renderer(glm::vec2 screenSize);

	Camera& getCamera();

	void render(Light light);
	void registerObject(Object* object);
	void update(float deltaTime);
};