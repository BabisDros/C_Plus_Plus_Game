#pragma once
#include "GameObject.h"
#include <string>
/*parameters:(float posX, float posY, float width, float height, float lifetime, float maxVelocity = 1.5f, float acceleration = 0.5f, float gravity = -1,
float oscillationFrequency = 5.0f, float oscillationAmplitude = 0.4f, float red = 1.0f, float green = 1.0f, float blue = 1.0f*/
class Particle:public GameObject
{
protected:
	graphics::Brush m_brush;
	std::string texture = "";
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
	float m_gravity = 0.0f;
	float m_oscillationFrequency = 0.f;
	float m_oscillationAmplitude = 0.0f;

public:
	Particle(float posX, float posY, float width, float height, float lifetime, std::string texture="", float maxVelocity = 1.5f, float acceleration = 0.5f,
		float gravity = -1.0f, float oscillationFrequency = 5.0f, float oscillationAmplitude = 0.4f, float red = 1.0f, float green = 1.0f, float blue = 1.0f);

	const bool isAlive() const;
	void init() override;
	void draw(bool drawWithOffset = true);
	void update(float dt) override;
	//all properties will run during lifetime
	void fade(const float& deltaTime);
	void move(const float& deltaTime);
	void reduceWidth(const float& deltaTime);
	void reduceHeight(const float& deltaTime);
	void reduceLife(const float& deltaTime);
	void oscilateInXAxis(const float& deltaTime);
};