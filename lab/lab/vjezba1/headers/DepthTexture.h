#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Light.h"

class DepthTexture {
private:
    GLuint depthMapFBO;
    GLuint depthMap;

    unsigned int width, height;

public:
    DepthTexture(unsigned int width = 1024, unsigned int height = 1024);
    ~DepthTexture();

    void bindForWriting();
    void unbind();

    GLuint getDepthMap() const;


};
