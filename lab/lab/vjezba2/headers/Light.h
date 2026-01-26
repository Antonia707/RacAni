#pragma once

#include <glm/glm.hpp>
#include "Transform.h"

class Light : public Transform
{
private:
    glm::vec3 intensity;
    glm::vec3 ambientIntensity;

public:
    Light();
    Light(glm::vec3 intensity, glm::vec3 ambientIntensity);
    ~Light();

    glm::vec3 getIntensity() const;
    glm::vec3 getAmbientIntensity() const;

    void setIntensity(const glm::vec3& intensity);
    void setAmbientIntensity(const glm::vec3& ambientIntensity);
};
