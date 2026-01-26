#include "Object.h"
#include "Shader.h"
#include <glm/gtx/string_cast.hpp>


Object::Object(Shader* shader, Renderable* renderable)
    : shader(shader), renderable(renderable) {
    material = nullptr;
    texture = nullptr;
}

Object::~Object() {
}

void Object::setShader(Shader* newShader) {
    shader = newShader;
}

void Object::setRenderable(Renderable* newRenderable) {
    renderable = newRenderable;
}

void Object::setMaterial(Material* newMaterial) {
    material = newMaterial;
}

void Object::setTexture(Texture* newTexture) {
    texture = newTexture;
}

Shader* Object::getShader() {
    return shader;
}

Renderable* Object::getRenderable() {
    return renderable;
}

Material* Object::getMaterial() {
    return material;
}

Texture* Object::getTexture() {
    return texture;
}

void Object::render(glm::mat4 perspectiveMatrix, glm::mat4 viewMatrix, glm::vec4 eyePosition, Light light, Shader* renderShader) {
    if (!renderShader || !renderable) return;

    renderShader->setUniform("model", getModelMatrix());
    renderShader->setUniform("view", viewMatrix);
    renderShader->setUniform("projection", perspectiveMatrix);
    renderShader->setUniform("eyePosition", eyePosition);

    renderShader->setUniform("lightProjection", light.getPerspectiveMatrix(glm::vec2(1000, 1000)));
    renderShader->setUniform("lightView", light.getViewMatrix());

    // sjencanje
    renderShader->setUniform("lightPosition", glm::vec3(light.getPosition()));
    renderShader->setUniform("lightColor", light.getIntensity());
    renderShader->setUniform("lightAmbient", light.getAmbientIntensity());

    if (material) {
        renderShader->setUniform("materialAmbient", material->getAmbientColor());
        renderShader->setUniform("materialDiffuse", material->getDifuzeColor());
        renderShader->setUniform("materialSpecular", material->getSpecularColor());
        renderShader->setUniform("materialShininess", material->getSpecularExponent());
    }

	renderShader->setUniform("lightDirection", glm::normalize(glm::vec3(light.getFront())));
    renderShader->setUniform("cutOff", light.getInnerCutoff());
    renderShader->setUniform("outerCutOff", light.getOuterCutoff());

	renderShader->setUniform("lineColor", getRenderable()->getColor());
    

    renderable->draw();
}
