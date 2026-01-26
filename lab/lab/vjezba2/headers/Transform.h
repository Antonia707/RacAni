#pragma once

#include <iostream>
#include <glm/glm.hpp>

class Transform
{

private:
	glm::vec4 position;
	glm::vec4 front;
	glm::vec4 up;
	glm::vec4 right;
	glm::vec4 scale;

public:
	Transform();
	virtual ~Transform() = default;

	void setPosition(const glm::vec3& pos);
	void setScale(const glm::vec3& scl);
	void setOrientation(const glm::vec3& newRight, const glm::vec3& newUp, const glm::vec3& newForward);

	glm::vec4 getPosition() { return position; }
	glm::vec4 getFront() { return front; }
	glm::vec4 getUp() { return up; }
	glm::vec4 getRight() { return right; }
	
	glm::mat4 getModelMatrix() const;
	glm::mat4 getViewMatrix() const;

	void globalMove(const glm::vec3& delta);
	void localMove(const glm::vec3& delta);

	void rotate(glm::mat4 rot);
	void rotateFPS(float xOffset, float yOffset, bool constrainPitch);

	void scaleF(const glm::vec3& factor);
	
	//void update(float deltaTime);
	// void registerAnimation(Lines krivulja);

	void print() const;
};
