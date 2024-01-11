#include "ParticleSystem.h"
#include "GameState.h"
#include <ctime>
#include <thread>
//Creates a system of individual particles 
ParticleSystem::ParticleSystem(int emissionRate, int maxParticles, float posX, float posY, float width, float particleSize, float lifetime, std::string texture, float maxVelocity,
    float acceleration, float gravity, float oscillationFrequency, float oscillationAmplitude, float red, float green, float blue) :
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
    m_blue(blue)
{}

ParticleSystem::~ParticleSystem()
{
    destroyParticles();
}

void ParticleSystem::init()
{      
    if (!isRunning())
    {
        destroyParticles();
        //used to start/reset particle system
        m_currentLife = m_lifetime;
    }     
}

void ParticleSystem::draw()
{
    if (isRunning())
    {
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
        //create particles in random position of the width of particle system
        //seed based on time
        srand(static_cast<unsigned int>(std::time(0)));
        float firstNumber = -2;
        int lastNumber =2;

        float randomPositionX = 0;
        // Spawn new particles based on emission rate. Particles per second
        m_emissionTimer += deltaTimeSec;
        while (m_emissionTimer > 1.0f / m_emissionRate)
        {
            if (m_particles.size() < m_maxParticles)
            {
                float randomVal = (firstNumber + rand() % lastNumber) / 10.f;
                //populate m_particles list with particles with random x position.
                randomPositionX = std::min(m_posX + m_width, m_posX + randomVal);
                randomPositionX = std::max(m_posX - m_width, randomPositionX);

                m_particles.push_back(new Particle(randomPositionX, m_posY, m_particleSize, m_particleSize, m_lifetime, m_texture, 
                    m_maxVelocity, m_acceleration, m_gravity, m_oscillationFrequency + randomVal, m_oscillationAmplitude + randomVal, m_red, m_green, m_blue));
            }
         
            // Reset the emission timer
            m_emissionTimer -= 1.0f / m_emissionRate;
        }
        // Update the system's life
        m_currentLife -= deltaTimeSec;

        std::thread updateThread(&ParticleSystem::updateThreadFunction, this, dt);
        updateThread.join();
    }  
}

void ParticleSystem::updateThreadFunction(float dt) 
{   
    for (auto& particle : m_particles) 
    {
        particle->setInitialPosition(m_posX, m_posY);
        particle->update(dt);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(5)); 
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
