#pragma once
#include "Entity.h"
#include "IDestructible.h"
#include "DamageBox.h"
#include "Ability.h"

class Enemy :public IDestructible, public Entity
{
	DamageBox m_projectile = DamageBox(false);
	Ability m_throwProjectile = Ability(5.f, 2.f, 0.f);
	const float m_gravity = 10.f;
	const float m_accel_vertical = 12.f;
	const float m_accel_horizontal = 12.f;	//? if its too slow, entity becomes unable to move (ex. starting next to a wall)
	const float m_max_velocity = 1.4f;

	float m_homebase_x;
	float m_homebase_y;

	float m_movement_range_x = 2.f;
	float m_movement_range_y = 2.f;

	int m_direction_y = 1; // 1: up | -1: down
	int m_movement_type = 2;	// 1: follow player | 2: static on x axis | 3: static on y axis | 4: static circling

	bool m_rangedAttack = true;
	float m_projectile_vx;
	const float m_projectile_accel_horizontal = 128.f;
	const float m_projectile_max_velocity = 4.f;
	float m_projectile_direction; //looking left value: -1 | looking right value: 1
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

	void attack(float dt);
	void rangedAttack(float dt);
};