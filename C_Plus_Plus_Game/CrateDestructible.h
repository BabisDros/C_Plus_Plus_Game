#pragma once
#include "GameObject.h"
#include "IDestructible.h"
#include "LevelBox.h"
class CrateDestructible:public GameObject,public LevelBox, public IDestructible
{
public:
	CrateDestructible(float x, float y, float w, float h, const std::string* texture, bool destructible) 
		: LevelBox(x, y, w, h, texture, destructible) {}
	void init() override;
	void draw() override;
	void update(float dt) override;

	void destroy() override;
	void instantiateParticles() override;
};