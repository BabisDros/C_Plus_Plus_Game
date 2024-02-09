#pragma once
#include "GameObject.h"
class Star:public GameObject
{
    float m_pos_x = 0.0f;
    float m_pos_y = 0.0f;
    float m_width = 0.0f;
    float m_height = 0.0f;
    float m_finalSize = 3.0f;

    float growthRate = 2.f; 

public:
    Star() {};
    Star(std::string name) :GameObject(name) {};
    void init() override;
    void draw() override;
    void update(const float& dt) override;
    bool hasGrown() const;
};