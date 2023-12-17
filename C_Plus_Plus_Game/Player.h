#pragma once
#include <sgg/graphics.h>
#include "GameObject.h"
#include "box.h"


class Player : public GameObject, public Box
{
	graphics::Brush m_brush_player;

public:
	Player(std::string name) : GameObject(name) {}

	void init() override;
	void draw() override;
	void update(float dt) override;
};