#pragma once
#include "GameObject.h"
//parameters:float posX, float posY, float width, float height, float life,float maxVelocity=1, float red=1, float green=1, float blue=1
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
	float m_acceleration= 0.5f;
	float m_lifetime = 0.0f;
	float m_currentLife = 0.0f;
	//set negative gravity to pull up object. Positive to pull down
	float m_gravity = -1.f;
	float oscillationFrequency = 5.0f;
	float oscillationAmplitude = 0.4f;

public:
	Particle(float posX, float posY, float width, float height, float life,float maxVelocity=2.f, float red=1.0f, float green=1.0f, float blue=1.0f );

	const bool isAlive() const;
	void init() override;
	void draw() override;
	void update(float dt) override;
	//all properties will run during lifetime
	void fade(const float& deltaTime);
	void move(const float& deltaTime);
	void setPosition(float x, float y, float width, float height);
	void reduceWidth(const float& deltaTime);
	void reduceHeight(const float& deltaTime);
	void reduceLife(const float& deltaTime);
	void oscilateInXAxis(const float& deltaTime);
};