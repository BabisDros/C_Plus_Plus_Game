#pragma once
#include <string>
#include <sgg/graphics.h>
class GameObject
{
	static int m_next_id;

protected:
	class GameState* m_state;
	int m_id = 0;
	bool m_active = true;
	graphics::Brush m_brush;
	//m_brush_debug not needed.debugDraw() method in util.h has a default brush. 
public:
	std::string m_name;
	GameObject(const std::string& name = "");
	virtual void init(){}
	virtual void draw(){}
	virtual void update(float dt){}
	virtual ~GameObject(){}
	bool isActive() const { return m_active; }
	void setActive(bool state) { m_active = state; }
	int getId() const { return m_id; }
};