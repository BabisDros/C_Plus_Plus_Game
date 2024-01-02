#pragma once
//INTERFACE. Objects that can be destructed should inherit this
class IDestructible 
{
public:
    IDestructible(int initialHealth):m_initialHealth(initialHealth), m_currentHealth(initialHealth) {};
    IDestructible() {};
    virtual ~IDestructible() {}
    virtual int getHealth() const = 0;
    virtual void resetHealth() = 0;
    virtual void takeDamage(const int& damage) = 0;
    virtual bool isAlive() const = 0;
    virtual void destroy() = 0;

protected:
    int m_initialHealth = 0;
    int m_currentHealth = 0;
};