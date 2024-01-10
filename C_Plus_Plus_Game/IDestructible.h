#pragma once
#include <iostream>//for debugging
#include "HealthUIMoving.h"
//INTERFACE. Objects that can be destructed should implement this
class IDestructible
{
public:
    HealthUIMoving* m_healthUi = nullptr;
    IDestructible(bool createHealthUI = true) : m_healthUi(createHealthUI ? new HealthUIMoving() : nullptr) { }
    ~IDestructible() {}

    virtual void setInitialHealthValues(const int& health);
    virtual void setHealth(const int& health);
    virtual int getHealth() const;
    virtual void resetHealth();
    virtual void takeDamage(const int& damage);
    virtual bool isAlive() const;
    //Object is not destroyed but setActive false
    virtual void destroy() = 0;
    virtual void instantiateParticles() = 0;
protected:
    int m_initialHealth = 0;
    int m_currentHealth = 0;
    //duration when the object can take anymore damage. Take damage once per hit
    float m_invincibilityDuration = 0.5f;
    float m_damageTakenTimestamp=0.0f;
    float m_points;
};