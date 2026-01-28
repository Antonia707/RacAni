#include <glm/ext/matrix_clip_space.hpp>

#include "Light.h"

Light::Light()
    : intensity(1.0f), ambientIntensity(0.1f){}

Light::Light(glm::vec3 intensity, glm::vec3 ambientIntensity)
    : intensity(intensity), ambientIntensity(ambientIntensity) {}

Light::~Light() {}

glm::vec3 Light::getIntensity() const {
    return intensity;
}

glm::vec3 Light::getAmbientIntensity() const {
    return ambientIntensity;
}

void Light::setIntensity(const glm::vec3& intensity) {
    this->intensity = intensity;
}

void Light::setAmbientIntensity(const glm::vec3& ambientIntensity) {
    this->ambientIntensity = ambientIntensity;
}

glm::mat4 Light::getPerspectiveMatrix(glm::vec2 screenSize) const {
    float aspect = screenSize.x / screenSize.y;
    float near = 0.1f;
    float far = 100.0f;

    float fovY = glm::radians(45.0f);
    float top = tan(fovY / 2.0f) * near;
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    return glm::frustum(left, right, bottom, top, near, far);
}

