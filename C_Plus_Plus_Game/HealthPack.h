#pragma once
#include "LevelBox.h"
#include "GameState.h"
#include "IDestructible.h"

class HealthPack :public LevelBox, IDestructible
{
public:
	int m_loot;
	bool m_extra_points = false;
	float m_heal_amount = -50; // reverse damage
	HealthPack(float x, float y, float w, float h, std::string* texture, bool destructible) // removed const
		:LevelBox(x, y, w, h, texture, destructible), IDestructible(false)
	{
		init();
	}

	void init() override;
	void draw() override;
	void update(const float& dt) override;
	void destroy() override;
};