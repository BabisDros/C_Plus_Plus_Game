#pragma once
#include "box.h"
#include "GameObject.h"

class CollisionObject:public GameObject, public Box 
{
public:
	CollisionObject() {};
	CollisionObject(float x, float y, float w, float h) : Box(x, y, w, h) {};
	CollisionObject(std::string name) : GameObject(name) {};
	CollisionObject(std::string name, float x, float y, float w, float h) : GameObject(name), Box(x, y, w, h) {}
};