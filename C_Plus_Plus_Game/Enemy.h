#pragma once
#include "Entity.h"
#include "IDestructible.h"

class Enemy :public IDestructible, public Entity
{
	const float m_gravity = 10.f;
	const float m_accel_vertical = 300.f;
	const float m_accel_horizontal = 10.f;	//? if its too slow, player becomes unable to move (ex. starting next to a wall)
	const float m_max_velocity = 1.4f;

	float m_homebase_x;
	float m_homebase_y;

	float m_movement_range_x = 3.f;
	float m_movement_range_y = 10.f;

public:
//	Enemy(std::string name, float width, float height, std::string& texture) : Entity(name) {}
	Enemy(std::string name) : Entity(name) {}
	void init() override;
	void draw() override;
	void update(float dt) override;

	void movement(float dt) override;
	void destroy() override;
	void instantiateParticles() override;
};