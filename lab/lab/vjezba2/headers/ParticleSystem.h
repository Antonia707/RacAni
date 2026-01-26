#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"
#include "Shader.h"
#include "Renderable.h"
#include "Material.h"
#include "Texture.h"


struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float life;       // remaining life in seconds
    float size;
};

class ParticleSystem : public Transform 
{
private:
    Shader* shader;
    Renderable* renderable;
    Material* material;
    Texture* texture;
    unsigned int numberOfParticles;
    unsigned int particleScale;
	unsigned int lifeSpan;
    glm::vec3 gravity;

public:
    // Constructor
    ParticleSystem(unsigned int maxParticles, Shader* particleShader, GLuint particleTexture);

    // Destructor
    ~ParticleSystem();

    // Update particle states (CPU-side optional, could be empty if fully GPU)
    void update(float deltaTime);

    // Render particles
    void render(const glm::mat4& view, const glm::mat4& projection);

    // Add new particles
    void emit(const glm::vec3& position, const glm::vec3& velocity,
        const glm::vec4& color, float size, float life, unsigned int count = 1);

    // Set general particle properties
    void setGravity(const glm::vec3& gravity);
    void setGlobalScale(float scale);

private:
    unsigned int m_maxParticles;
    Shader* m_shader;
    GLuint m_texture;

    std::vector<Particle> m_particles;

    // OpenGL buffers
    GLuint m_VAO;
    GLuint m_VBO;          // quad vertex buffer
    GLuint m_instanceVBO;  // per-instance particle data

    // Internal helper
    void initBuffers();
    void updateInstanceBuffer();

    // Particle properties
    glm::vec3 m_gravity;
    float m_globalScale;
};
