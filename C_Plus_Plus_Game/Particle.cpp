#include "Particle.h"
#include "GameState.h"

void Particle::init()
{
	m_state = GameState::getInstance();
	m_currentLife = m_lifetime;
}

void calcStep()
{

}
void Particle::draw()
{
	
}

void Particle::update(float dt)
{
	if (isAlive())
	{
		float deltaTime = dt / 1000.0f;
		fade(deltaTime);
		move(deltaTime);
		reduceSize(deltaTime);
		reduceLife(deltaTime);
		oscilateInXAxis(deltaTime);
	}
	
}

const bool Particle::isAlive() const
{
	return m_currentLife > 0;
}

void Particle::fade(const float& deltaTime)
{
	m_brush.fill_opacity -= deltaTime;
}

void Particle::move(const float& deltaTime)
{	
	m_velocity = std::min(m_maxVelocity, m_velocity + deltaTime * m_acceleration);
	m_velocity = std::max(-m_maxVelocity, m_velocity);

	m_velocity += deltaTime * m_gravity;
	m_pos_y += deltaTime * m_velocity;
}


void Particle::reduceSize(const float& deltaTime)
{
	
}

void Particle::reduceLife(const float& deltaTime)
{
	
	m_currentLife -= deltaTime;
	

}

void Particle::oscilateInXAxis(const float& deltaTime)
{

}
