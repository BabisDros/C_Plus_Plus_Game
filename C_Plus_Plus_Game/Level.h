#pragma once
#include "GameObject.h"
#include <sgg/graphics.h>
#include <list>

class Level : public GameObject
{
	graphics::Brush m_brush_background;

	float m_center_x = 3.0f;
	float m_center_y = 3.0f;
	
	// everything included in the level
	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_dynamic_objects;

public:
	void init() override;
	void draw() override;
	void update(float dt) override;
	Level(const std::string& name = "Level0");
	~Level();
};