#pragma once

#include <string>
#include <glad/glad.h>

class Texture {
private:
    GLuint textureID = 0;
    int width = 0;
    int height = 0;
    int nrChannels = 0;

public:
    Texture() = default;
    Texture(const std::string& path);

    bool loadFromFile(const std::string& path);
    int getTextureID() const;

};
