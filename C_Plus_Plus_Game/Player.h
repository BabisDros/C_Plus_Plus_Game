#pragma once
#include <sgg/graphics.h>
#include "GameObject.h"
#include "box.h"
#include "timer.h"
#include "Destructible.h"
#include "DamageBox.h"
#include "Ability.h"

class Player : public GameObject, public Box, public Destructible
{
	graphics::Brush m_brush_player;
	
	Timer m_myTimer;
	const float m_gravity = 10.f;
	const float m_accel_vertical = 300.f;
	const float m_accel_horizontal = 15.f;	//? if its too slow, player becomes unable to move (ex. starting next to a wall)
	const float m_max_velocity = 5.0f;

	Ability dashAbility=Ability(2.0f, 0.3f, 0.0f, 15.f);
	Ability slashAbility = Ability(2.0f, 0.1f, 0.0f);
	Ability jumpAbility = Ability(1.0f, 0.0f, 0.0f);
	//looking left value:-1,looking right value:1
	int m_lookingDirection = 1;
	DamageBox damageBox;
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
	
	int getHealth() const override;
	void setInitialHealth(const int& initialHealth) override;
	void takeDamage(const int& damage) override;
	bool isAlive() const override;
	void destroy() override;

	bool m_collidingUp = false;
//current velocities
	float m_vx = 0.f; 
	float m_vy = 0.f;

	float m_camera_multiplier_x = 0.f;	// what part of the screen player should be on X axis
	float m_camera_multiplier_y = 0.f;	// what part of the screen player should be on Y axis

protected:
	void cameraOffsetX(float multiplier1, float multiplier2);
	void cameraOffsetY(float multiplier1, float multiplier2);

};