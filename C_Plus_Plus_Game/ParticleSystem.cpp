#include "ParticleSystem.h"
#include "GameState.h"
#include <ctime>
#include <thread>
#include <iostream>

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

void ParticleSystem::draw(bool drawWithOffset)
{
    if (isRunning())
    {
        //should lock every m_particles because it is used by both threads
        std::lock_guard<std::mutex> lock(particlesMutex);
        for (Particle*& particle : m_particles)
        {
            particle->draw(drawWithOffset);
        }
    }
}

void ParticleSystem::update(float dt, bool playOnPaused)
{
    if (!playOnPaused && m_state->getCurrentState() == Paused) return;
    if (m_currentLife > 0)
    {      
        float deltaTimeSec = dt / 1000;

        // Spawn new particles based on emission rate. Particles per second
        m_emissionTimer += deltaTimeSec;
        while (m_emissionTimer > 1.0f / m_emissionRate)
        {
            if (m_particles.size() < m_maxParticles)
            {
                float randomVal = calcRandomValue();
                std::lock_guard<std::mutex> lock(particlesMutex);
                m_particles.push_back(new Particle(calcRandomPosX(), m_posY, m_particleSize, m_particleSize, m_lifetime, m_texture,
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
        particle->update(dt);
    }
}
//TODO:HANDLE small width values under 1
float ParticleSystem::calcRandomPosX() const
{
    float firstNumber = 0;
    int lastNumber= 10;
    float randomVal = (firstNumber + rand() % lastNumber)/10.f;

    float randomPositionX = m_posX - m_width/2 + randomVal * m_width;

    return randomPositionX;
}

float ParticleSystem::calcRandomValue() 
{
    float firstNumber = -2;
    int lastNumber = 2;
    float randomVal = (firstNumber + rand() % lastNumber) / 10.f;
   
    return randomVal;
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
