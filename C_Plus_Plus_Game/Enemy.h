#pragma once
#include "Entity.h"
#include "IDestructible.h"

class Enemy :public IDestructible, public Entity
{
	const float m_gravity = 10.f;
	const float m_accel_vertical = 300.f;
	const float m_accel_horizontal = 12.f;	//? if its too slow, entity becomes unable to move (ex. starting next to a wall)
	const float m_max_velocity = 1.4f;

	float m_homebase_x;
	float m_homebase_y;

	float m_movement_range_x = 2.f;
	float m_movement_range_y = 2.f;

	int m_movement_type = 3;	// 1: follow player | 2: static on x axis | 3: static on y axis | 4: static circling
	int m_direction = 1; // 1: right, -1: left
public:
//	Enemy(std::string name, float width, float height, std::string& texture) : Entity(name) {}
	Enemy(std::string name, float pos_x, float pos_y) : Entity(name) {
		m_pos_x = pos_x;
		m_pos_y = pos_y;
	}
	void init() override;
	void draw() override;
	void update(float dt) override;

	void movement(float dt) override;
	void destroy() override;
	void instantiateParticles() override;

	void movementStaticX(float dt);
	void movementStaticY(float dt);
	void movementDynamic(float dt);
};