#include "particleSystem.h"
#include "Shader.h" // Your Shader class that handles shader compilation and uniforms
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>
#include <cstdlib>

// Quad vertices (2D square, centered at origin, for billboarding)
static float quadVertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 1.0f,
     0.5f,  0.5f, 1.0f, 1.0f
};

ParticleSystem::ParticleSystem(unsigned int maxParticles, Shader* particleShader, GLuint particleTexture)
    : m_maxParticles(maxParticles), m_shader(particleShader), m_texture(particleTexture),
    m_gravity(0.0f, -9.81f, 0.0f), m_globalScale(1.0f)
{
    m_particles.reserve(maxParticles);
    initBuffers();
}

ParticleSystem::~ParticleSystem()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_instanceVBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void ParticleSystem::initBuffers()
{
    // Generate VAO and VBOs
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Quad vertex buffer
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Vertex positions (layout = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // Texture coords (layout = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Instance buffer
    glGenBuffers(1, &m_instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW);

    // Layout 2: position (vec3)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    glVertexAttribDivisor(2, 1);

    // Layout 3: color (vec4)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
    glVertexAttribDivisor(3, 1);

    // Layout 4: size (float)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, size));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);
}

void ParticleSystem::emit(const glm::vec3& position, const glm::vec3& velocity,
    const glm::vec4& color, float size, float life, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i)
    {
        if (m_particles.size() >= m_maxParticles)
            break;

        Particle p;
        p.position = position;
        p.velocity = velocity;
        p.color = color;
        p.size = size;
        p.life = life;

        m_particles.push_back(p);
    }
}

void ParticleSystem::update(float deltaTime)
{
    for (auto& p : m_particles)
    {
        if (p.life > 0.0f)
        {
            p.velocity += m_gravity * deltaTime;
            p.position += p.velocity * deltaTime;
            p.life -= deltaTime;

            // Optional: fade color alpha over lifetime
            p.color.a = std::max(0.0f, p.life / 5.0f); // assuming max life ~5s
        }
    }

    // Remove dead particles
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
            [](const Particle& p) { return p.life <= 0.0f; }),
        m_particles.end()
    );

    // Update instance buffer
    updateInstanceBuffer();
}

void ParticleSystem::updateInstanceBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particles.size() * sizeof(Particle), m_particles.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::render(const glm::mat4& view, const glm::mat4& projection)
{
    if (m_particles.empty())
        return;

    m_shader->use();
    m_shader->setUniform("uView", view);
    m_shader->setUniform("uProjection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    m_shader->setUniform("uTexture", 0);

    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(m_particles.size()));
    glBindVertexArray(0);
}
