#include "BezierBuilder.h"
#include <cmath>

glm::vec3 BezierBuilder::bernstein(float t, std::vector<glm::vec3> cp) {
    int n = cp.size() - 1;
    glm::vec3 point(0.0f);

    for (int i = 0; i <= n; ++i) {
        float binomial = glm::pow(1 - t, n - i) * glm::pow(t, i) * (float)std::tgamma(n + 1) / (std::tgamma(i + 1) * std::tgamma(n - i + 1));
        point += binomial * cp[i];
    }

    return point;
}

std::vector<glm::vec3> BezierBuilder::makeApproximationCurve(int samples, std::vector<glm::vec3> dots) {
    std::vector<glm::vec3> curvePoints;

    for (int i = 0; i <= samples; ++i) {
        float t = (float)i / samples;
        curvePoints.push_back(bernstein(t, dots));
    }

    return curvePoints;
}


std::vector<glm::vec3> BezierBuilder::makeInterpolationCurve(std::vector<glm::vec3> dots) {
    size_t n = dots.size();
    std::vector<glm::vec3> control;
    if (n == 0) return control;

    if (n == 1) {
        control.push_back(dots[0]);
        return control;
    }
    if (n == 2) {
        control.push_back(dots[0]);
        control.push_back(dots[1]);
        return control;
    }
    if (n == 3) {
        control.push_back(dots[0]);
        control.push_back(dots[1]);
        control.push_back(dots[2]);
        return control;
    }

    glm::vec3 P0 = dots[n - 4];
    glm::vec3 P1 = dots[n - 3];
    glm::vec3 P2 = dots[n - 2];
    glm::vec3 P3 = dots[n - 1];

    glm::vec3 B0 = P0;
    glm::vec3 B3 = P3;

    float t1 = 1.0f / 3.0f;
    float t2 = 2.0f / 3.0f;
    float u1 = 1.0f - t1;
    float u2 = 1.0f - t2;

    float b10 = 3 * u1 * u1 * t1;
    float b11 = 3 * u1 * t1 * t1;
    float b20 = 3 * u2 * u2 * t2;
    float b21 = 3 * u2 * t2 * t2;

    glm::vec3 rhs1 = P1 - (u1 * u1 * u1) * B0 - (t1 * t1 * t1) * B3;
    glm::vec3 rhs2 = P2 - (u2 * u2 * u2) * B0 - (t2 * t2 * t2) * B3;

    float det = b10 * b21 - b11 * b20;
    if (std::abs(det) < 1e-6f) {
        return control;
    }

    glm::vec3 B1 = (b21 * rhs1 - b11 * rhs2) / det;
    glm::vec3 B2 = (-b20 * rhs1 + b10 * rhs2) / det;

    control = { B0, B1, B2, B3 };
    return control;
}

