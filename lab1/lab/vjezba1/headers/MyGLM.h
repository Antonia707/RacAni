#pragma once

#include <glm/fwd.hpp>

class MyGLM
{
public:
	static glm::mat4 translate3D(glm::vec3 translateVector);
	static glm::mat4 scale3D(glm::vec3 scaleVector);
	static glm::mat4 rotate3D(glm::vec3 axis, float angle);
	static glm::mat4 lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp);
};

