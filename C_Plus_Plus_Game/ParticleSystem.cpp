#include "ParticleSystem.h"
#include <ctime>
#include <iostream>//TODO:debug

ParticleSystem::ParticleSystem(int emissionRate, int maxParticles, float posX, float posY, float width, float particleSize, float lifetime, std::string texture, float maxVelocity,
    float acceleration, float gravity, float oscillationFrequency, float oscillationAmplitude, float red, float green, float blue ):
    m_emissionRate(emissionRate),
    m_maxParticles(maxParticles),
    m_posX(posX),
    m_posY(posY),
    m_width(width),
    m_particleSize(particleSize),
    m_lifetime(lifetime),
    m_texture(texture),  
    m_maxVelocity(maxVelocity),
    m_acceleration(acceleration),
    m_gravity(gravity),
    m_oscillationFrequency(oscillationFrequency),
    m_oscillationAmplitude(oscillationAmplitude),
    m_red(red),
    m_green(green),
    m_blue(blue),
    m_currentLife(lifetime)
{
}

ParticleSystem::~ParticleSystem()
{
    std::cout << "delete";
    for (Particle* particle : m_particles)
    {
        delete particle;
    }
    m_particles.clear();
}

void ParticleSystem::draw()
{
    if (m_currentLife > 0)
    {
        for (auto& particle : m_particles)
        {
            particle->draw();
        }
    }
   
}


void ParticleSystem::update(float dt)
{
   
    if (m_currentLife > 0)
    {
        
        //create particles in random position of the width of particle system
   //seed based on time
        srand(static_cast<unsigned int>(std::time(0)));
        float firstNumber = 1;
        int lastNumber =10;

        float randomPositionX = 0;
        // Spawn new particles based on emission rate
        m_emissionTimer += dt/1000;
        while (m_emissionTimer > 1.0f / m_emissionRate)
        {
            
            //populate m_particles list with particles with random x position.
            randomPositionX =std::min(m_posX+m_width,m_posX+(firstNumber + rand()% lastNumber)/100);
            m_particles.push_back(new Particle(randomPositionX, m_posY, m_particleSize, m_particleSize, m_lifetime, m_texture, m_maxVelocity, m_acceleration, m_gravity,
                m_oscillationFrequency, m_oscillationAmplitude, m_red, m_green, m_blue));

            // Reset the emission timer
            m_emissionTimer -= 1.0f / m_emissionRate;
        }
        // Update the system's life
        m_currentLife -= dt/1000;
        // Update existing particles
        for (auto& particle : m_particles)
        {
            particle->setPosition(m_posX, m_posY);
            particle->update(dt);
        }
    }  
   
}

bool ParticleSystem::isRunning() const
{
    if (m_currentLife <= 0)
    {
        return false;
    }

    else if (m_currentLife < m_lifetime)
    {
        return true;
    }

    else return false;
}

void ParticleSystem::followParentGameobject(float x, float y)
{
    m_posX = x;
    m_posY = y;
}
