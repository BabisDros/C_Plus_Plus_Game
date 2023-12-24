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


	void movePlayer(float dt);
	
public:
	Player(std::string name) : GameObject(name) {}

	void init() override;
	void draw() override;
	void update(float dt) override;

//current velocities
	float m_vx = 0.f; 
	float m_vy = 0.f;

	float m_camera_offset_x = 0.f;
	float m_camera_offset_y = 0.f;

protected:
	void debugDraw();

};