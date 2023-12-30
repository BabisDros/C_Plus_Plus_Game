#pragma 
/** \param cooldown, duration, startTime, speed=0
*/
class  Ability
{
private:
	float m_cooldown;
	float m_duration;
	float m_startTime;
	float m_speed;
public:

	Ability(float cooldown, float duration, float startTime, float speed=0)
		:m_cooldown(cooldown), m_duration(duration), m_startTime(startTime), m_speed(speed) {};

	void setStartTime(float time)
	{
		m_startTime = time;
	}
	float getStartTime()const
	{
		return m_startTime ;
	}
	float getDuration()const
	{
		return m_duration;
	}
	float getCooldown()const
	{
		return m_cooldown;
	}
	float getSpeed()const
	{
		return m_speed;
	}

	bool isRunning()const
	{
		return m_startTime != 0;
	}
};

