#include "ParticleSystem.h"
ParticleSystem::ParticleSystem(int emissionRate, int maxParticles, float posX, float posY, float width, float height, float lifetime, std::string texture, float maxVelocity,
    float acceleration, float gravity, float oscillationFrequency, float oscillationAmplitude, float red, float green, float blue ):
    m_emissionRate(emissionRate),
    m_maxParticles(maxParticles),
    m_posX(posX),
    m_posY(posY),
    m_width(width),
    m_height(height),
    m_lifetime(lifetime),
    m_texture(texture),  
    m_maxVelocity(maxVelocity),
    m_acceleration(acceleration),
    m_gravity(gravity),
    m_oscillationFrequency(oscillationFrequency),
    m_oscillationAmplitude(oscillationAmplitude),
    red(red),
    green(green),
    blue(blue)
{
    init();
}

ParticleSystem::~ParticleSystem()
{
    for (Particle* particle : m_particles)
    {
        delete particle;
    }
}

void ParticleSystem::init()
{
    for (int i = 0; i < m_maxParticles; i++)
    {
        m_particles.push_back(new Particle(m_posX, m_posY, m_width, m_height, m_lifetime, m_texture, m_maxVelocity, m_acceleration, m_gravity,
            m_oscillationFrequency, m_oscillationAmplitude, red, green, blue));
    }
}

void ParticleSystem::update(float dt)
{
    for (auto& particle : m_particles)
    {
        particle->update(dt);
    }
}
