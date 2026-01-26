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
