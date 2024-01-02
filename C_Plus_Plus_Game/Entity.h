#pragma once
#include "GameObject.h"
#include "box.h"
class Entity : public Box, public GameObject
{
public:
	virtual void checkCollision();
	Entity(std::string name) : GameObject(name) {}

protected:
	bool m_mirrored = false;
	float m_vx = 0.f;
	float m_vy = 0.f;
};