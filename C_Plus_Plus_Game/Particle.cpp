#include "Particle.h"
#include "GameState.h"
#include "util.h"

Particle::Particle(float posX, float posY, float width, float height, float life, float red, float green, float blue)
{
	SETCOLOR(m_brush.fill_color, red, green, blue);
	m_initialPosX = posX;
	m_initialPosY = posY;
	m_initialWidth = width;
	m_initialHeight = height;
	m_lifetime = m_currentLife = life;
	
	init();
}

void Particle::init()
{
	m_state = GameState::getInstance();
}

void calcStep()
{

}
void Particle::draw()
{
	graphics::drawRect(m_currentPosX + m_state->m_global_offset_x, m_currentPosY + m_state->m_global_offset_y, m_currentWidth, m_currentHeight, m_brush);
}

void Particle::update(float dt)
{
	if (isAlive())
	{
		float deltaTime = dt / 1000.0f;
		fade(deltaTime);
		move(deltaTime);
		reduceWidth(deltaTime);
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
	//1.f is the initial opacity
	m_brush.fill_opacity -=1.f/m_lifetime* deltaTime;
}

void Particle::move(const float& deltaTime)
{	
	m_velocity = std::min(m_maxVelocity, m_velocity + deltaTime * m_acceleration);
	m_velocity = std::max(-m_maxVelocity, m_velocity);

	m_velocity += deltaTime * m_gravity;
	m_currentPosX += deltaTime * m_velocity;
}


void Particle::reduceWidth(const float& deltaTime)
{
	m_currentWidth = m_initialWidth / m_lifetime * deltaTime;
}

void Particle::reduceHeight(const float& deltaTime)
{
	m_currentHeight = m_initialHeight / m_lifetime * deltaTime;
}

void Particle::reduceLife(const float& deltaTime)
{	
	m_currentLife -= deltaTime;
}

void Particle::oscilateInXAxis(const float& deltaTime)
{

}
