#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>

#include "Lines.h"

class BSpline : public Lines {
private:
	struct Vec3Comparator {
		bool operator()(const glm::vec3& a, const glm::vec3& b) const {
			if (a.x != b.x) return a.x < b.x;
			if (a.y != b.y) return a.y < b.y;
			return a.z < b.z;
		}
	};
	std::map<glm::vec3, glm::vec3, Vec3Comparator> tangents;	// point, tangent
	std::map<glm::vec3, glm::vec3, Vec3Comparator> binormals;	// point, normal
public:
	struct Orientation {
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};

	BSpline(int samples, std::vector<glm::vec3> points);
	std::vector<std::vector<glm::vec3>> getTangentLines(int step, float length);
	Orientation getOrientation(glm::vec3 point);
};
