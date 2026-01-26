#pragma once

#include <glm/glm.hpp>

class Material
{
private:
    glm::vec3 ambientColor;
    glm::vec3 difuzeColor;
    glm::vec3 specularColor;
    float specularExponent;

public:
    Material();
    Material(glm::vec3 ambientColor, glm::vec3 difuzeColor, glm::vec3 specularColor, float specularExponent = 32.0f);

    glm::vec3 getAmbientColor();
    glm::vec3 getDifuzeColor();
    glm::vec3 getSpecularColor();
    float getSpecularExponent();

    void setAmbientColor(glm::vec3& color);
    void setDifuzeColor(glm::vec3& color);
    void setSpecularColor(glm::vec3& color);
    void setSpecularExponent(float exponent);
};

