#pragma once
#include "GameObject.h"
#include <sgg/graphics.h>
#include <list>
#include "box.h"
#include <map>
#include <tuple>
#include "LevelBox.h"

class Level : public GameObject
{
	graphics::Brush m_brush_background;

	float m_center_x = 3.0f;
	float m_center_y = 3.0f;

	//? everything included in the level
	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_dynamic_objects;
	std::vector<LevelBox> m_blocks;

	std::map <char, std::tuple <float, float, const std::string , bool>> m_objects_data;	//? For every tag, width, height, texture and is destructible are saved

	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;

public:
	void init() override;
	void draw() override;
	void update(float dt) override;
	Level(const std::string& name = "Level 0");
	~Level();

	void checkCollisions();
	void drawBlock(int i);
	void pausedDraw();
	void read();

	std::vector<Box>* getBlocks()
	{
		return &m_blocks;
	}
};