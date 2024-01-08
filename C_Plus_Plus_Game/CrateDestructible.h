#pragma once
#include "IDestructible.h"
#include "LevelBox.h"

class CrateDestructible:public LevelBox, public IDestructible
{
public:
	CrateDestructible(int initialHealth, float x, float y, float w, float h, const std::string* texture, bool destructible) 
		: LevelBox(x, y, w, h, texture, destructible) 
	{
		init();
		setInitialHealthValues(initialHealth);
		m_healthUi->setPosition(x,y);
		m_can_die = true;
	}
	~CrateDestructible() 
	{
		if (m_healthUi)
			delete m_healthUi;
	}
	void init() override;
	void draw() override;

	void destroy() override;
	void instantiateParticles() override;
};