#pragma once
//INTERFACE. Objects that can be destructed, should inherit this
class Destructible 
{
public:
    Destructible(int initialHealth):m_initialHealth(initialHealth), m_currentHealth(initialHealth) {};
    Destructible() {};
    virtual ~Destructible() {}
    virtual int getHealth() const = 0;
    virtual void setInitialHealth(const int& initialHealth) = 0;
    virtual void takeDamage(const int& damage) = 0;
    virtual bool isAlive() const = 0;
    virtual void destroy() = 0;

    // Additional methods can be added as needed
protected:
    int m_initialHealth = 0;
    int m_currentHealth = 0;
};