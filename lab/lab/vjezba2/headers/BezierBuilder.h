#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Lines.h"

class BezierBuilder {
public:
    static glm::vec3 bernstein(float t, std::vector<glm::vec3> cp);
    static std::vector<glm::vec3> makeApproximationCurve(int samples, std::vector<glm::vec3> dots);
    static std::vector<glm::vec3> makeInterpolationCurve(std::vector<glm::vec3> dots);
};
