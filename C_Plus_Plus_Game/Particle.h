#pragma once
#include "GameObject.h"
//parameters:(float red, float green, float blue, float posX, float posY, float width, float height,float life)
class Particle:public GameObject
{
	graphics::Brush m_brush;
	
	float m_initialPosX = 0.0f;
	float m_initialPosY = 0.0f;
	float m_initialWidth = 0.0f;
	float m_initialHeight = 0.0f;
	float m_currentPosX = 0.0f;
	float m_currentPosY = 0.0f;
	float m_currentWidth = 0.0f;
	float m_currentHeight = 0.0f;
	float m_velocity = 0.0f;
	float m_maxVelocity = 0.0f;
	float m_acceleration= 0.0f;
	float m_lifetime = 0.0f;
	float m_currentLife = 0.0f;
	float m_gravity = 0.0f;
	float m_oscilationWidth = 0.0f;
	float m_oscilationReduceStep = 0.0f;

public:
	Particle(float posX, float posY, float width, float height, float life, float red=1, float green=1, float blue=1 );

	const bool isAlive() const;
	void init() override;
	void draw() override;
	void update(float dt) override;
	//all properties will run during lifetime
	void fade(const float& deltaTime);
	void move(const float& deltaTime);
	void reduceWidth(const float& deltaTime);
	void reduceHeight(const float& deltaTime);
	void reduceLife(const float& deltaTime);
	void oscilateInXAxis(const float& deltaTime);
};