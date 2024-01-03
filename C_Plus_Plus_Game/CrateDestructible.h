#pragma once
#include "IDestructible.h"
#include "LevelBox.h"

class CrateDestructible:public LevelBox, public IDestructible
{

public:
	HealthUI* healthUi;
	CrateDestructible(int initialHealth, float x, float y, float w, float h, const std::string* texture, bool destructible) 
		: LevelBox(x, y, w, h, texture, destructible) 
	{
		init();
		setHealthValues(initialHealth);
		healthUi = new HealthUI(x, y);
	}

	void init() override;
	void draw() override;

	void takeDamage(const int& damage) override;
	void destroy() override;
	void instantiateParticles() override;
};