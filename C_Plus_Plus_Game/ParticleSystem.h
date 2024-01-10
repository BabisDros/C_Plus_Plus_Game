#pragma once
#include <vector>
#include "Particle.h"
/*int emissionRate, int maxParticles, float posX, float posY, float width, float particleSize, float lifetime, std::string texture="", float maxVelocity = 1.5f, float acceleration = 0.5f,
		float gravity = -1,float oscillationFrequency = 5.0f, float oscillationAmplitude = 0.4f, float red = 1.0f, float green = 1.0f, float blue = 1.0f*/
class ParticleSystem:public GameObject
{			
	
	graphics::Brush m_brush;	
	std::vector<Particle*> m_particles;
	
	//number of particles emmited per second
	int m_emissionRate = 0;
	int m_maxParticles = 0;	
	float m_posX = 0.0f;
	float m_posY = 0.0f;
	//the width from which the particles will appear in random position.
	float m_width = 0.0f;
	float m_particleSize = 0.0f;
	float m_lifetime = 0.0f;
	std::string m_texture = "";
	float m_maxVelocity = 0.0f;
	float m_acceleration = 0.5f;
	//set negative gravity to pull up object | Positive to pull down | 0 for stationary
	float m_gravity = 0;
	float m_oscillationFrequency = 0.f;
	float m_oscillationAmplitude = 0.f;
	//default color white
	float m_red = 1.0f;
	float m_green = 1.0f;
	float m_blue = 1.0f;

	float m_currentLife = 0;
	float m_emissionTimer = 0;
public:
	ParticleSystem(int emissionRate, int maxParticles, float posX, float posY, float width, float particleSize, float lifetime, std::string texture = "", float maxVelocity = 1.5f, float acceleration = 1.0f,
		float gravity = 1, float oscillationFrequency = 5.0f, float oscillationAmplitude = 0.4f, float red = 1.0f, float green = 1.0f, float blue = 1.0f);
	~ParticleSystem();
	//call this so that the particles start playing
	void init() override;
	void draw() override;
	void update(float dt) override;
	bool isRunning() const;
	void followGameobject(float x, float y);
	void destroyParticles();
};