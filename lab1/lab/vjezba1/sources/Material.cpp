#include "Material.h"

Material::Material()
    : ambientColor(1.0f), difuzeColor(1.0f), specularColor(1.0f), specularExponent(32.0f){}

Material::Material(glm::vec3 ambientColor, glm::vec3 difuzeColor, glm::vec3 specularColor, float specularExponent)
    : ambientColor(ambientColor), difuzeColor(difuzeColor), specularColor(specularColor), specularExponent(specularExponent){}


glm::vec3 Material::getAmbientColor() {
    return ambientColor;
}

glm::vec3 Material::getDifuzeColor() {
    return difuzeColor;
}

glm::vec3 Material::getSpecularColor() {
    return specularColor;
}

float Material::getSpecularExponent() {
    return specularExponent;
}


void Material::setAmbientColor(glm::vec3& color) {
    ambientColor = color;
}

void Material::setDifuzeColor(glm::vec3& color) {
    difuzeColor = color;
}

void Material::setSpecularColor(glm::vec3& color) {
    specularColor = color;
}

void Material::setSpecularExponent(float exponent) {
    specularExponent = exponent;
}
