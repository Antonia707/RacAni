#include <glm/ext/matrix_clip_space.hpp>

#include "Camera.h"

void Camera::setZoom(float z)
{
	zoom = z;
	if (zoom < 0.1f) {
		zoom = 0.1f;
	}
	else if (zoom > 45.0f) {
		zoom = 45.0f;
	}
}

float Camera::getZoom() const
{
    return zoom;
}

glm::mat4 Camera::getPerspectiveMatrix(glm::vec2 screenSize) const {
    float aspect = screenSize.x / screenSize.y;
    float near = 0.1f;
    float far = 100.0f;

    float tanHalfFov = tan(glm::radians(zoom) / 2.0);
    float top = tanHalfFov * near;
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    return glm::frustum(left, right, bottom, top, near, far);
}

