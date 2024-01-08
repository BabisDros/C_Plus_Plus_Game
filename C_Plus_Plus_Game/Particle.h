#pragma once
#include "GameObject.h"

class Particle:public GameObject
{
	graphics::Brush m_brush;
	
	float m_pos_x = 0.0f;
	float m_pos_y = 0.0f;
	float m_width = 0.0f;
	float m_height = 0.0f;
	float m_velocity = 0.0f;
	float m_maxVelocity = 0.0f;
	float m_acceleration= 0.0f;
	float m_lifetime = 0.0f;
	float m_currentLife = 0.0f;
	float m_gravity = 0.0f;
	float m_oscilationWidth = 0.0f;
	float m_oscilationReduceStep = 0.0f;

public:
	Particle(float red, float green, float blue, float posX, float posY, float width, float height);

	const bool isAlive() const;
	void init() override;
	void draw() override;
	void update(float dt) override;
	//all properties will run during lifetime
	void fade(const float& deltaTime);
	void move(const float& deltaTime);
	void reduceSize(const float& deltaTime);
	void reduceLife(const float& deltaTime);
	void oscilateInXAxis(const float& deltaTime);
};