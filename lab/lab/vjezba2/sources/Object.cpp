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

void Object::render(Camera camera, Light light, glm::vec2 screenSize) {
    if (!shader || !renderable) return;

    shader->use();

    glm::mat4 modelMatrix = getModelMatrix();

    shader->setUniform("model", modelMatrix);
    shader->setUniform("view", camera.getViewMatrix());
    shader->setUniform("projection", camera.getPerspectiveMatrix(screenSize));
    shader->setUniform("eyePosition", camera.getPosition());
    shader->setUniform("cameraRight", camera.getRight());
    shader->setUniform("cameraUp", camera.getUp());

    // sjencanje
    shader->setUniform("lightPosition", glm::vec3(light.getPosition()));
    shader->setUniform("lightColor", light.getIntensity());
    shader->setUniform("lightAmbient", light.getAmbientIntensity());

    if (material) {
        shader->setUniform("materialAmbient", material->getAmbientColor());
        shader->setUniform("materialDiffuse", material->getDifuzeColor());
        shader->setUniform("materialSpecular", material->getSpecularColor());
        shader->setUniform("materialShininess", material->getSpecularExponent());
    }
  
    renderable->draw();
}
