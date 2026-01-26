#include <vector>
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>

#include "BSpline.h"


BSpline::BSpline(int samples, std::vector<glm::vec3> points) 
    : Lines (std::vector<glm::vec3>(), glm::vec3(1.0f, 1.0f, 1.0f)) {
    int n = points.size();
    int degree = 3;
    int nSegments = n - degree;

    glm::mat4 B = {
        {-1.f / 6.f,  3.f / 6.f, -3.f / 6.f, 1.f / 6.f},
        { 3.f / 6.f, -6.f / 6.f,  3.f / 6.f, 0.f},
        {-3.f / 6.f,  0.f,        3.f / 6.f, 0.f},
        { 1.f / 6.f,  4.f / 6.f,  1.f / 6.f, 0.f}
    };

    for (int i = 0; i < nSegments; ++i) {
        glm::mat4x3 R = { points[i], points[i + 1], points[i + 2], points[i + 3] };

        for (int s = 0; s < samples; ++s) {
            std::vector<glm::vec3> tangent;
            float t = (float)(s) / samples;
            glm::vec4 T = { t * t * t, t * t, t, 1.0f };
            glm::vec4 dT = { 3.f * t * t, 2.f * t, 1.f, 0.f };
			glm::vec4 ddT = { 6.f * t, 2.f, 0.f, 0.f };

            glm::vec3 p = R * (B * T);
            glm::vec3 d = R * (B * dT);
			glm::vec3 bn = R * (B * ddT);

			addPoint(p);
			tangents.insert({ p, glm::normalize(d) });
			binormals.insert({ p, glm::normalize(bn) });
        }
    }
}

std::vector<std::vector<glm::vec3>> BSpline::getTangentLines(int step, float length) {
    std::vector<std::vector<glm::vec3>> tangentList;

    for (int i = 0; i < getVertices().size(); i += step) {
        std::vector<glm::vec3> tangent;
        glm::vec3 p = getVertices()[i];
        tangent.push_back(p);
		tangent.push_back(p + length * tangents[p]);
		tangentList.push_back(tangent);
    }
    return tangentList;
}

BSpline::Orientation BSpline::getOrientation(glm::vec3 point) {
    if (tangents.empty()) {
        return { glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0) };
    }

    glm::vec3 tangent = tangents.at(point);
	glm::vec3 binormal = binormals.at(point);
    glm::vec3 normal = glm::normalize(glm::cross(binormal, tangent));

    return { tangent, normal, binormal };
}
