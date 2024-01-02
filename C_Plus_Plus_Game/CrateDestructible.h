#pragma once
#include "GameObject.h"
#include "IDestructible.h"

class CrateDestructible:public GameObject,public IDestructible
{
public:
	CrateDestructible(std::string name) : GameObject(name) {}
	void init() override;
	void draw() override;
	void update(float dt) override;

	int getHealth() const override;
	void resetHealth() override;
	void takeDamage(const int& damage) override;
	bool isAlive() const override;
	void destroy() override;

};