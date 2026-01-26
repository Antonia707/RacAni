#pragma once

#include <glm/glm.hpp>

#include "Transform.h"

class Camera : public Transform {
private:
	float zoom = 45.0f;

public:
	Camera() = default;

	void setZoom(float z);
	float getZoom() const;

	glm::mat4 getPerspectiveMatrix(glm::vec2 screenSize) const;
};