#pragma once

/** \param cooldown, duration, startTime, speed=0
*/
class  Ability
{
private:
	float m_cooldown=0;
	float m_duration=0;
	float m_startTime=0;
	float m_speed=0;
public:

	Ability(float cooldown, float duration, float startTime, float speed=0)
		:m_cooldown(cooldown), m_duration(duration), m_startTime(startTime), m_speed(speed) {};

	float getElapsedTime();
	void resetIfCooldownExpired();
	void setStartTime(float time);
	float getStartTime()const;
	float getDuration()const;
	float getCooldown()const;
	float getSpeed()const;
	bool isRunning()const;
};

