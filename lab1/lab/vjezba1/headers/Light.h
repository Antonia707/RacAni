#pragma once

#include <glm/glm.hpp>
#include "Transform.h"

class Light : public Transform
{
private:
    glm::vec3 intensity;
    glm::vec3 ambientIntensity;
    float innerCutoff;
    float outerCutoff;

public:
    Light();
    Light(glm::vec3 intensity, glm::vec3 ambientIntensity);
    ~Light();

    glm::vec3 getIntensity() const;
    glm::vec3 getAmbientIntensity() const;

    void setIntensity(const glm::vec3& intensity);
    void setAmbientIntensity(const glm::vec3& ambientIntensity);

	void setInnerCutoff(float innerCutoff) { this->innerCutoff = innerCutoff; }
	void setOuterCutoff(float outerCutoff) { this->outerCutoff = outerCutoff; }
	float getInnerCutoff() const { return innerCutoff; }
	float getOuterCutoff() const { return outerCutoff; }

    glm::mat4 getPerspectiveMatrix(glm::vec2 screenSize) const;
};
