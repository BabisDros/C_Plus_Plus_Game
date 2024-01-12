#include "ParticleSystem.h"
#include "GameState.h"
#include <ctime>
#include <thread>

ParticleSystem::~ParticleSystem()
{
    destroyParticles();
}
//init is used to reset and run the system
void ParticleSystem::init()
{      
    if (!isRunning())
    {
        std::lock_guard<std::mutex> lock(particlesMutex);
        destroyParticles();
        //used to start/reset particle system
        m_currentLife = m_lifetime;
    }     
}

void ParticleSystem::draw()
{
    if (isRunning())
    {
        //should lock every m_particles because it is used by both threads
        std::lock_guard<std::mutex> lock(particlesMutex);
        for (Particle*& particle : m_particles)
        {
            particle->draw();
        }
    }
}

void ParticleSystem::update(float dt)
{
    if (m_state->getCurrentState() == Paused) return;
    if (m_currentLife > 0)
    {      
        float deltaTimeSec = dt / 1000;

        // Spawn new particles based on emission rate. Particles per second
        m_emissionTimer += deltaTimeSec;
        while (m_emissionTimer > 1.0f / m_emissionRate)
        {
            if (m_particles.size() < m_maxParticles)
            {
                float randomVal = calcRandomPositionX();

                m_particles.push_back(new Particle(calcRandomPositionX(), m_posY, m_particleSize, m_particleSize, m_lifetime, m_texture,
                    m_maxVelocity, m_acceleration, m_gravity, m_oscillationFrequency + randomVal, m_oscillationAmplitude + randomVal, m_red, m_green, m_blue));
            }
         
            // Reset the emission timer
            m_emissionTimer -= 1.0f / m_emissionRate;
        }
        // Update the system's life
        m_currentLife -= deltaTimeSec;
        std::lock_guard<std::mutex> lock(particlesMutex);
        std::thread updateThread(&ParticleSystem::updateThreadFunction, this, dt);
        updateThread.join();
    }  
}

void ParticleSystem::updateThreadFunction(float dt)
{
    for (auto& particle : m_particles)
    {
        particle->setInitialPosition(calcRandomPositionX(), m_posY);
        particle->update(dt);
    }
}

float ParticleSystem::calcRandomPositionX() 
{
    float firstNumber = -2;
    int lastNumber = 2;

    float randomVal = (firstNumber + rand() % lastNumber) / 10.f;
    float randomPositionX = std::min(m_posX + m_width, m_posX + randomVal);
    randomPositionX = std::max(m_posX - m_width, randomPositionX);
    return randomPositionX;
}

bool ParticleSystem::isRunning() const
{
    return (m_currentLife > 0 && m_currentLife < m_lifetime);
}

void ParticleSystem::followGameobject(float x, float y)
{
    m_posX = x;
    m_posY = y;
}

void ParticleSystem::destroyParticles()
{  
    if (m_particles.size() > 0)
    {
        for (Particle* particle : m_particles)
        {
            delete particle;
        }

        m_particles.clear();
    }  
}
