#pragma once
#include <sgg/graphics.h>
#include "GameObject.h"
#include "IDestructible.h"
#include "DamageBox.h"
#include "Ability.h"
#include "Entity.h"
#include "ParticleSystem.h"

enum AnimationSequence
{
	Idle,
	Walking,
	Attacking,
	Jumping,
	Dashing
};


class Player :public IDestructible, public Entity
{
	DamageBox m_slashWeapon = DamageBox(10);
	Ability m_dashAbility = Ability(2.0f, 0.2f, 0.0f, 12.f);
	Ability m_slashAbility = Ability(1.0f, 0.25f, 0.0f);
	Ability m_jumpAbility = Ability(0.3f, 0.0f, 0.0f);

	const float m_gravity = 10.f;
	const float m_accel_vertical = 300.f;
	const float m_accel_horizontal = 15.f;	//? if its too slow, player becomes unable to move (ex. starting next to a wall)
	const float m_max_velocity = 5.0f;	
	
	bool m_dev_fly = false;
	bool m_dev_fly_active = false;
	bool m_dev_fly_held = false;

	std::vector<std::string> m_sprites_walking;
	std::vector<std::string> m_sprites_idle;
	std::vector<std::string> m_sprites_attacking;
	std::vector<std::string> m_sprites_jumping;
	std::vector<std::string> m_sprites_dashing;
	std::vector<std::string>* m_sprites_ptr;
	float jump();
	void fly(float dt);
	void dash(float dt);
	void slash(float dt);
	void takeDamage(const int& damage) override;

	bool m_allow_animation_change;
	static AnimationSequence m_animation;
	float m_animation_timer = *GameState::getInstance()->getPausableClock();
	Ability m_jumpAnimation = Ability(0.6f, 0.0f, 0.0f);	// Making use of their pausable clocks to play the animations
	Ability m_attackAnimation = Ability(0.3f, 0.0f, 0.0f);
	Ability m_dashAnimation = Ability(0.2f, 0.0f, 0.0f);

	bool m_being_pushed;
	float m_being_pushed_timer;
	float m_pushed_x;
	float m_pushed_y;
public:
	Player(std::string name,float initialHealth) : Entity(name), IDestructible(false)
	{
		setInitialHealthValues(initialHealth);
		m_height = 1.f;
		m_width = 0.65f;
	}
	~Player();
	void init() override;
	void draw() override;
	void update(float dt) override;
	
	void movement(float dt) override;
	void destroy() override;
	void pickAnimation();
	
	void setPushed(float x, float y);
	void getPushed(float dt);
	
protected:
	void cameraOffsetX(float multiplier1, float multiplier2);
	void cameraOffsetY(float multiplier1, float multiplier2);

	void readSprites(std::string folder, std::vector<std::string> &myVec);
};