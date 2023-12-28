#pragma once
#include <sgg/graphics.h>
#include "GameObject.h"
#include "box.h"
#include "timer.h"

class Player : public GameObject, public Box
{
	graphics::Brush m_brush_player;
	Timer m_myTimer;
	const float m_gravity = 10.f;
	const float m_accel_vertical = 300.f;
	const float m_accel_horizontal = 10.f;
	const float m_max_velocity = 5.0f;

	//dash attributes
	int m_dash_cooldown = 2.f;
	float m_dashDuration = 0.3f;
	float m_dashStartTime = 0.f;
	float m_dashSpeed = 15.f;
	//dash attributes

	bool m_mirrored = false;
	bool m_dev_fly = false;
	bool m_dev_fly_active = false;
	bool m_dev_fly_held = false;

	void movePlayer(float dt);
	void dash(float dt);
	void fly(float dt);
public:
	Player(std::string name) : GameObject(name) {}

	void init() override;
	void draw() override;
	void update(float dt) override;
	bool m_collidingUp = false;
//current velocities
	float m_vx = 0.f; 
	float m_vy = 0.f;

	float m_camera_multiplier_x = 0.f;	// what part of the screen player should be on X axis
	float m_camera_multiplier_y = 0.f;	// what part of the screen player should be on Y axis

protected:
	void debugDraw();
	void cameraOffsetX(float multiplier1, float multiplier2);
	void cameraOffsetY(float multiplier1, float multiplier2);

};