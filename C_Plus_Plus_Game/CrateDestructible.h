#pragma once
#include "IDestructible.h"
#include "LevelBox.h"

enum Loot
{
	Nothing,
	Extra_loot,
	Health_pack
};

class CrateDestructible:public LevelBox, public IDestructible
{
public:
	Loot m_loot;
	bool m_extra_points = false;
	bool m_has_health = false;
	std::string m_health_texture = "Health.png"; // removed const

	CrateDestructible(int initialHealth, float x, float y, float w, float h, std::string* texture, bool destructible, Loot loot) //removed const
		: LevelBox(x, y, w, h, texture, destructible), m_loot(loot)
	{
		init();
		setInitialHealthValues(initialHealth);
		m_healthUi->setPosition(x,y);
		m_can_die = true;
		m_points = 10;
	}
	~CrateDestructible() {} // m_healthUi should only be deleted in by its' own class, so IDestructible

	void init() override;
	void draw() override;
	void destroy() override;
};