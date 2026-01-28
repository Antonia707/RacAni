#include <iostream>

#include "Renderer.h"
#include "InputManager.h"

Renderer::Renderer(glm::vec2 screenSize) : screenSize(screenSize) {
	depthShader = nullptr;
}

Camera& Renderer::getCamera()
{
	return camera;
}

Shader* Renderer::getDepthShader() {
    return depthShader;
}

void Renderer::setDepthShader(Shader* shader) {
	if (shader) {
		depthShader = shader;
	}
	else {
		std::cerr << "Error: Attempted to set a null shader." << std::endl;
	}
}

void Renderer::registerObject(Object* object) {
    objects.push_back(object);
}

void Renderer::depthRender(Light light) {
    glUseProgram(depthShader->ID);
    depthShader->use();
    depthMap->bindForWriting();
    for (auto& obj : objects) {
        if (depthShader) {
            obj->render(camera.getPerspectiveMatrix(screenSize), camera.getViewMatrix(), camera.getPosition(), light, depthShader);
        }
    }
    depthMap->unbind();
}

void Renderer::render(Light light) {
    glViewport(0, 0, InputManager::WIDTH, InputManager::HEIGHT);
    glClearColor(0.6039f, 0.9020f, 0.9294f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto& obj : objects) {
        if (obj && obj->getRenderable()) {
            glUseProgram(obj->getShader()->ID);
            obj->getShader()->use();
            if (obj->getTexture()) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, obj->getTexture()->getTextureID());
                glUniform1i(glGetUniformLocation(obj->getShader()->ID, "diffuseTexture"), 0);
            }
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap->getDepthMap());
            obj->getShader()->setUniform("shadowMap", 1);

            obj->render(camera.getPerspectiveMatrix(screenSize), camera.getViewMatrix(), camera.getPosition(), light, obj->getShader());
        }
    }
}
