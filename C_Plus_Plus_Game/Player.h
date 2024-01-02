#pragma once
#include <sgg/graphics.h>
#include "GameObject.h"
#include "box.h"
#include "timer.h"
#include "IDestructible.h"
#include "DamageBox.h"
#include "Ability.h"

class Player : public GameObject, public Box, public IDestructible
{
	DamageBox damageBox;
	Ability dashAbility = Ability(2.0f, 0.1f, 0.0f, 23.f);
	Ability slashAbility = Ability(1.0f, 0.1f, 0.0f);
	Ability jumpAbility = Ability(0.3f, 0.0f, 0.0f);

	const float m_gravity = 10.f;
	const float m_accel_vertical = 300.f;
	const float m_accel_horizontal = 15.f;	//? if its too slow, player becomes unable to move (ex. starting next to a wall)
	const float m_max_velocity = 5.0f;	
	//looking left value:-1,looking right value:1
	int m_lookingDirection = 1;
	
	bool m_mirrored = false;
	bool m_dev_fly = false;
	bool m_dev_fly_active = false;
	bool m_dev_fly_held = false;

	void movePlayer(float dt);
	float jump();
	void fly(float dt);
	void dash(float dt);
	void slash(float dt);
	
public:
	Player(std::string name) : GameObject(name) {}
	void init() override;
	void draw() override;
	void update(float dt) override;
	
	void takeDamage(const int& damage) override;
	void destroy() override;
	void instantiateParticles() override;
	bool m_collidingUp = false;
//current velocities
	float m_vx = 0.f; 
	float m_vy = 0.f;

protected:
	void cameraOffsetX(float multiplier1, float multiplier2);
	void cameraOffsetY(float multiplier1, float multiplier2);

};