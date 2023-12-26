#pragma once
#include <sgg/graphics.h>
#include "GameObject.h"
#include "box.h"


class Player : public GameObject, public Box
{
	graphics::Brush m_brush_player;

	const float m_gravity = 10.f;
	const float m_accel_vertical = 300.f;
	const float m_accel_horizontal = 40.f;
	const float m_max_velocity = 10.0f;

	bool m_mirrored = false;
	bool m_dev_fly = false;
	bool m_dev_fly_active = false;
	bool m_dev_fly_held = false;

	void movePlayer(float dt);
	void fly(float dt);
public:
	Player(std::string name) : GameObject(name) {}

	void init() override;
	void draw() override;
	void update(float dt) override;

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