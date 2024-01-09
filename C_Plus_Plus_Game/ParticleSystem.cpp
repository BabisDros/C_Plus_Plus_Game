#include "ParticleSystem.h"
#include <ctime>
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
    //create particles in random position of the width of particle system
    //seed based on time
    srand(static_cast<unsigned int>(std::time(0)));
    float firstNumber = 0.5f;
    float widthOfRange = m_width;
    

    for (int i = 0; i < m_maxParticles; i++)
    {
        float randomPositionX = firstNumber + rand() / widthOfRange;
        m_particles.push_back(new Particle(randomPositionX, m_posY, m_width, m_height, m_lifetime, m_texture, m_maxVelocity, m_acceleration, m_gravity,
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
