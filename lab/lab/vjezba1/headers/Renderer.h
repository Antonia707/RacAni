#pragma once
#include <vector>
#include <iostream>

#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "DepthTexture.h"

class Renderer
{
private:
	std::vector<Object*> objects;
	Camera camera;
	Shader* depthShader;
	DepthTexture* depthMap;

	glm::vec2 screenSize;

public:
	Renderer(glm::vec2 screenSize);

	Camera& getCamera();

	Shader* getDepthShader();
	void setDepthShader(Shader* shader);

	DepthTexture* getDepthMap() const { return depthMap; }
	void setDepthMap(DepthTexture* depthMap) { this->depthMap = depthMap; }

	void depthRender(Light light);
	void render(Light light);
	void registerObject(Object* object);
	void update(float deltaTime);
};