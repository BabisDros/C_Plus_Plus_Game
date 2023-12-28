#pragma once
//used as interface
class Destructible 
{
public:
    virtual ~Destructible() {}

    virtual int getHealth() const = 0;

    virtual void setInitialHealth(const int& initialHealth) = 0;

    virtual void takeDamage(const int& damage) = 0;

    virtual bool isAlive() const = 0;

    virtual void destroy() = 0;

    // Additional methods can be added as needed
protected:
    float m_initialHealth = 0;
    float m_currentHealth = 0;
};