#include "Ability.h"
#include "GameState.h"
float Ability::getElapsedTime()
{
	return *GameState::getInstance()->getPausableClock() - m_startTime;
}

void Ability::resetIfCooldownExpired()
{
	if (getElapsedTime() >= getCooldown())
	{
		setStartTime(0.f);
	}
}

void Ability::setStartTime(const float& time)
{
	m_startTime = time;
}

float Ability::getStartTime() const
{
	return m_startTime;
}

float Ability::getDuration() const
{
	return m_duration;
}

float Ability::getCooldown() const
{
	return m_cooldown;
}

float Ability::getSpeed() const
{
	return m_speed;
}

bool Ability::isRunning() const
{
	return m_startTime != 0;
}
