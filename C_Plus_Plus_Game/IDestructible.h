#pragma once
//INTERFACE. Objects that can be destructed should implement this
class IDestructible 
{
public:
    IDestructible(int initialHealth):m_initialHealth(initialHealth), m_currentHealth(initialHealth) {};
    IDestructible() {};
    virtual ~IDestructible() {}
    virtual int getHealth() const { return m_currentHealth; };
    virtual void resetHealth() { m_currentHealth = m_initialHealth; };
    virtual void takeDamage(const int& damage) { m_currentHealth -= damage; };
    virtual bool isAlive() const { return m_currentHealth > 0; };
    virtual void destroy() = 0;
    virtual void instantiateParticles() = 0;

protected:
    int m_initialHealth = 0;
    int m_currentHealth = 0;
};