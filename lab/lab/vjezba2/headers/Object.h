#pragma once

#include <glm/glm.hpp>

#include "Transform.h"
#include "Shader.h"
#include "Renderable.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"
#include "Camera.h"


class Object : public Transform
{
private:
	Shader* shader;
	Renderable* renderable;
	Material* material;
	Texture* texture;

public:
	Object() = default;
	Object(Shader* shader, Renderable* renderable);
	~Object();
	
	void setShader(Shader* shader);
	void setRenderable(Renderable* renderable);
	void setMaterial(Material* material);
	void setTexture(Texture* texture);

	Shader* getShader();
	Renderable* getRenderable();
	Material* getMaterial();
	Texture* getTexture();

	void render(Camera camera, Light light, glm::vec2 screenSize);
};
