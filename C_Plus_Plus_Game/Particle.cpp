#include "Particle.h"
#include "GameState.h"
#include "util.h"

Particle::Particle(float posX, float posY, float width, float height, float lifetime, std::string texture, float maxVelocity, float acceleration,
	float gravity, float oscillationFrequency, float oscillationAmplitude, float red, float green, float blue):
	m_initialPosX(posX), m_currentPosX(posX),
	m_initialPosY(posY), m_currentPosY(posY),
	m_initialWidth(width), m_currentWidth(width),
	m_initialHeight(height), m_currentHeight(height),
	m_lifetime(lifetime), m_currentLife(lifetime),
	m_maxVelocity(maxVelocity),m_acceleration(acceleration),
	m_gravity(gravity),
	m_oscillationFrequency(oscillationFrequency),
	m_oscillationAmplitude(oscillationAmplitude)
{		
	SETCOLOR(m_brush.fill_color, red, green, blue);
	m_brush.outline_opacity = 0;
	m_brush.texture = texture;
	init();
}

void Particle::init()
{
	m_state = GameState::getInstance();
}

void Particle::draw()
{
	graphics::drawRect(m_currentPosX + m_state->m_global_offset_x, m_currentPosY + m_state->m_global_offset_y, m_currentWidth, m_currentHeight, m_brush);

	if (m_state->m_debugging)
	{
		debugDraw(m_currentPosX + m_state->m_global_offset_x, m_currentPosY + m_state->m_global_offset_y, m_currentWidth, m_currentHeight, m_id);
	}
}

void Particle::update(float dt)
{
	if (isAlive())
	{
		float deltaTime = dt / 1000.0f;
		fade(deltaTime);
		move(deltaTime);
		reduceWidth(deltaTime);
		reduceHeight(deltaTime);
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
	m_velocity = std::min(m_maxVelocity, m_velocity + deltaTime*m_gravity * m_acceleration);
	m_velocity = std::max(-m_maxVelocity, m_velocity);
	//std::cout << "MAX velocity " << m_maxVelocity<< "velocity " << m_velocity << std::endl;
	m_currentPosY += deltaTime * m_velocity;
}


void Particle::reduceWidth(const float& deltaTime)
{
	m_currentWidth -= m_initialWidth / m_lifetime * deltaTime;
}

void Particle::reduceHeight(const float& deltaTime)
{
	m_currentHeight -= m_initialHeight / m_lifetime * deltaTime;
}

void Particle::reduceLife(const float& deltaTime)
{	
	m_currentLife -= deltaTime;
}

void Particle::oscilateInXAxis(const float& deltaTime)
{
	// Current life is to reduce oscilation near death of particle
	float oscillation = std::sin(m_oscillationFrequency * m_currentLife) * m_oscillationAmplitude;
	m_currentPosX += oscillation * deltaTime;
}
