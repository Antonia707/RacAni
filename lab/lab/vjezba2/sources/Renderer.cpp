#include <iostream>

#include "Renderer.h"

Renderer::Renderer(glm::vec2 screenSize) : screenSize(screenSize) {

}

Camera& Renderer::getCamera()
{
	return camera;
}

void Renderer::registerObject(Object* object) {
    objects.push_back(object);
}

void Renderer::render(Light light) {
    for (auto& obj : objects) {
        if (obj && obj->getRenderable()) {
            glUseProgram(obj->getShader()->ID);
			if (obj->getTexture()) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getTextureID());
                glUniform1i(glGetUniformLocation(obj->getShader()->ID, "diffuseTexture"), 0);;
			}
            obj->render(camera, light, screenSize);
        }
    }
}

void Renderer::update(float deltaTime) {
    // TO DO
}
