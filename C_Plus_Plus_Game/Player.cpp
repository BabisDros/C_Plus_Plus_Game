#pragma once
#include "Player.h"
#include "util.h"
#include "Level.h"
#include "CallbackManager.h"
#include <filesystem>

AnimationSequence Player::m_animation = Idle;
Player::~Player()
{
}

void Player::init()
{
	if (!isActive()) setActive(true);
	m_pos_x = m_state->getLevel()->m_player_start_x;
	m_pos_y = m_state->getLevel()->m_player_start_y;

	setCustomBrushProperties(&m_brush, 1.0f, 0.0f, m_state->getFullAssetPath("Character Sprites V2\\Idle\\Idle1.png"));

	graphics::Brush slash;
	setCustomBrushProperties(&slash, 1.0f, 0.0f, m_state->getFullAssetPath("slashFx.png"));
	
	m_slashWeapon.setBrush(slash);
	m_slashWeapon.setParentIsPlayer(true);
	m_slashWeapon.setParentDirection(m_lookingDirection);

	//trigger callbackmanager to display health value
	CallbackManager::getInstance()->m_playerHealthChanged.trigger(IDestructible::m_initialHealth, IDestructible::m_currentHealth);
	CallbackManager::getInstance()->m_playerLivesChanged.trigger(0, false);//used 0 to initialize the ui
}

void Player::draw()
{
	if (m_mirrored) graphics::setScale(-1.0f, 1.0f); //mirrors image

	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 1.4f, 1.2f, m_brush);

	graphics::resetPose(); //reset mirror for next call

	if (m_state->m_debugging)
	{
		debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, m_id);
	}	
	m_slashWeapon.draw();	
}

void Player::update(float dt)
{
	if (m_jumpAnimation.isRunning()) m_jumpAnimation.resetIfCooldownExpired();
	if (m_attackAnimation.isRunning()) m_attackAnimation.resetIfCooldownExpired();
	if (m_dashAnimation.isRunning()) m_dashAnimation.resetIfCooldownExpired();
	if (!m_jumpAnimation.isRunning() && !m_attackAnimation.isRunning() && !m_dashAnimation.isRunning())
	{
		m_allow_animation_change = true;
	}
	m_collidingDown = false;
	m_collidingUp = false;
	checkCollision(m_state->getLevel()->getBlocks());
	checkCollision(m_state->getLevel()->getDestructibleObjects());

	m_slashWeapon.setParentDirection(m_lookingDirection);
	m_slashWeapon.update(dt);
	float delta_time = dt / 1000.0f;

	m_state->enable(m_dev_fly, m_dev_fly_held, graphics::getKeyState(graphics::SCANCODE_9));
	if (m_dev_fly) fly(delta_time);
	else movement(delta_time);

	if (m_pos_y > m_state->getCanvasHeight() + m_state->getCanvasHeight()*0.5f + 1) //? is in void
	{
		takeDamage(m_initialHealth);
		m_pos_x = m_state->getLevel()->m_player_start_x; 
		m_pos_y = m_state->getLevel()->m_player_start_y;
	}

	cameraOffsetX(0.4f, 0.6f);
	cameraOffsetY(0.4f, 0.6f);

	dash(delta_time);
	slash(delta_time);

	pickAnimation();
	float dif = *GameState::getInstance()->getPausableClock() - m_animation_timer;	// change texture
	m_brush.texture = (*m_sprites_ptr).at((int)(8 * dif) % (*m_sprites_ptr).size());
	
	// sound
	if (m_collidingDown && m_animation == Walking) 
	{ 
		graphics::playSound("music\\Footstep_Dirt_03.mp3", 0.01f);
	}
}

void Player::destroy()
{
	CallbackManager::getInstance()->m_playerLivesChanged.trigger(-1, false);
	if (m_state->m_lives == 0)
	{
		setActive(false);
		m_state->setState(Lose);
		std::filesystem::remove("data\\save file.txt");
	}
	else
	{		
		CallbackManager::getInstance()->m_playerDied.trigger();
	}
}

void Player::pickAnimation()
{
	if (m_animation == Idle)
	{
		m_sprites_ptr = &m_state->m_sprites_idle;
	}
	else if (m_animation == Walking)
	{
		m_sprites_ptr = &m_state->m_sprites_walking;
	}
	else if (m_animation == Attacking)
	{
		m_sprites_ptr = &m_state->m_sprites_attacking;
	}
	else if (m_animation == Jumping)
	{
		m_sprites_ptr = &m_state->m_sprites_jumping;
	}
	else if (m_animation == Dashing)
	{
		m_sprites_ptr = &m_state->m_sprites_dashing;
	}
	else if (m_animation == Hurt)
	{
		m_sprites_ptr = &m_state->m_sprites_hurt;
	}
}

void Player::setPushedState(bool isPushed)
{
	m_being_pushed = isPushed;
}

void Player::setPushed(float x, float y)
{
	if (x > m_pos_x) m_pushed_x = -1;
	else m_pushed_x = 1;
	if (y > m_pos_y) m_pushed_y = -1;
	else m_pushed_y = 1;	// get pushed in oposite direction of intersection
	
	m_being_pushed_timer = *GameState::getInstance()->getPausableClock();
	m_being_pushed = true;
	// damage sound
	graphics::playSound("music\\Body_Flesh_8.wav", 0.04f);
}

void Player::getPushed(float delta_time)
{
	if (*GameState::getInstance()->getPausableClock() - m_being_pushed_timer > 0.2f)
	{
		m_being_pushed = false;
	}
	else
	{
		m_vx = m_max_velocity / 4.f * m_pushed_x;
		m_vy = m_max_velocity / 5.f * m_pushed_y;
		m_pos_x += delta_time * m_vx;
		m_pos_y += delta_time * m_vy;
		m_animation = Hurt;
	}
}

Ability* Player::getDashAbility()
{
	return &m_dashAbility;
}

void Player::movement(float delta_time)
{
	float move = 0;

	if (graphics::getKeyState(graphics::SCANCODE_LEFT))	//? movement
	{
		move = -1.0f;
		m_mirrored = true;
		m_lookingDirection = -1;
	}
	if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
	{
		move = 1.0f;
		m_mirrored = false;
		m_lookingDirection = 1;
	}

	if (m_animation == Jumping && m_collidingUp) m_allow_animation_change = true;	// do not play jump animation when player hits ceiling

	if (graphics::getKeyState(graphics::SCANCODE_LEFT) ^ graphics::getKeyState(graphics::SCANCODE_RIGHT)) //? insta stop
	{
		m_vx = std::min(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
		m_vx = std::max(-m_max_velocity, m_vx);
		if (m_allow_animation_change)
		{ 
			if (m_animation != Walking) m_animation_timer = *GameState::getInstance()->getPausableClock();
			m_animation = Walking;
		}		
	}
	else
	{
		m_vx = 0.f;
		if (m_allow_animation_change)
		{
			if (m_animation != Idle) m_animation_timer = *GameState::getInstance()->getPausableClock();
			m_animation = Idle;
		}
	}

	m_pos_x += delta_time * m_vx;
	CallbackManager::getInstance()->m_playerMoved.trigger(m_pos_x, m_pos_y);
	m_vy -= jump();

	m_vy = std::min(m_max_velocity, m_vy);
	m_vy = std::max(-m_max_velocity, m_vy);
	
	if (m_dashAbility.isRunning())	// add gravity if dash is not waiting for cooldown and it isn't in it's duration
	{
		if(m_dashAbility.getElapsedTime() > m_dashAbility.getDuration())
		{ 
			m_vy += delta_time * m_gravity;
		}
		else
		{
			m_vy = 0;
		}
	}
	else
	{
		m_vy += delta_time * m_gravity;
	}
	m_pos_y += delta_time * m_vy;

	if (m_being_pushed) getPushed(delta_time);
}

float Player::jump()
{
	float accel = 0;
	if (m_vy == 0.0f && graphics::getKeyState(graphics::SCANCODE_UP) && !m_jumpAbility.isRunning())
	{
		m_jumpAbility.setStartTime(*m_state->getPausableClock());
		accel = m_accel_vertical * 0.02f;
		m_jumpAnimation.setStartTime(*m_state->getPausableClock());
		m_animation = Jumping;
		m_animation_timer = *GameState::getInstance()->getPausableClock();
		m_allow_animation_change = false;
	}
	
	if (m_jumpAbility.isRunning())
	{	
		m_jumpAbility.resetIfCooldownExpired();
	}

	return accel;
}

void Player::fly(float delta_time)
{
	const float velocity = 10.0f;
	if (graphics::getKeyState(graphics::SCANCODE_LEFT))	//? movement
	{
		m_pos_x -= velocity * delta_time;
		m_mirrored = true;
	}
	if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
	{
		m_pos_x += velocity * delta_time;
		m_mirrored = false;
	}
	if (graphics::getKeyState(graphics::SCANCODE_UP))
	{
		m_pos_y -= velocity * delta_time;
	}
	if (graphics::getKeyState(graphics::SCANCODE_DOWN))
	{
		m_pos_y += velocity * delta_time;
	}
}

void Player::dash(float delta_time)
{
	if (graphics::getKeyState(graphics::SCANCODE_Z) && !m_dashAbility.isRunning())
	{
		m_dashAbility.setStartTime(*m_state->getPausableClock());
		m_dashAnimation.setStartTime(*m_state->getPausableClock());
		m_animation = Dashing;
		m_animation_timer = *GameState::getInstance()->getPausableClock();
		m_allow_animation_change = false;
	}	
	
	if (m_dashAbility.isRunning())
	{	
		//dash has certain duration, otherwise character teleports to another position and creates bugs like passing through objects
		if (m_dashAbility.getElapsedTime() < m_dashAbility.getDuration())
		{
			m_vx = m_dashAbility.getSpeed() * m_lookingDirection;
			m_pos_x += delta_time * m_vx;
		}
		m_dashAbility.resetIfCooldownExpired();
	}
}

void Player::slash(float delta_time)
{
	if (graphics::getKeyState(graphics::SCANCODE_X) && !m_slashAbility.isRunning())
	{	
		m_slashWeapon.setActive(true);
		m_slashAbility.setStartTime(*m_state->getPausableClock());
		m_attackAnimation.setStartTime(*m_state->getPausableClock());
		m_animation = Attacking;
		m_animation_timer = *GameState::getInstance()->getPausableClock();
		m_allow_animation_change = false;
		//sound
		graphics::playSound("music\\Light_Sword_Swing_3.wav", 0.05f);
	}
	if (m_slashAbility.isRunning())
	{
		if (m_slashAbility.getElapsedTime() < m_slashAbility.getDuration())
		{
			//extra offset (0.5f) corrected with the player's direction
			float lookingDirOffset = 0.5f * m_lookingDirection;
			//slash damagebox follows player
			m_slashWeapon.setPosition(m_pos_x + lookingDirOffset, m_pos_y , 0.8f, 0.8f);
		}
		else if(m_slashWeapon.isActive())
		{			
			m_slashWeapon.setActive(false);
		}

		m_slashAbility.resetIfCooldownExpired();
	}
}

void Player::takeDamage(const int& damage)
{
	//call base class to update health
	IDestructible::takeDamage(damage);

	//triggers the methods with two arguments and with empty
	CallbackManager::getInstance()->m_playerHealthChanged.trigger( IDestructible::m_initialHealth, IDestructible::m_currentHealth);
	if (damage>0) CallbackManager::getInstance()->m_playHurtFx.trigger();
}


void Player::cameraOffsetX(float multiplier1, float multiplier2)
{															//? 0.5f is based on zoom done on background, check level draw/init
	if (m_pos_x >= m_state->getCanvasWidth()*(multiplier1 - 0.5f) && (m_pos_x <= m_state->getCanvasWidth() * (multiplier2 + 0.5f))) //? prevents going outside of background
	{
		if (m_pos_x + m_state->m_global_offset_x <= m_state->getCanvasWidth() * multiplier1)
		{
			m_state->m_global_offset_x = m_state->getCanvasWidth() * multiplier1 - m_pos_x;	
		}
		else if (m_pos_x + m_state->m_global_offset_x >= m_state->getCanvasWidth() * multiplier2)
		{
			m_state->m_global_offset_x = m_state->getCanvasWidth() * multiplier2 - m_pos_x;
		}
	}
	else if (m_pos_x < m_state->getCanvasWidth() * (multiplier1 - 0.5f))
	{
		m_state->m_global_offset_x = m_state->getCanvasWidth() * 0.5f;
	}
	else if (m_pos_x > m_state->getCanvasWidth() * (multiplier2 + 0.5f))
	{
		m_state->m_global_offset_x = -m_state->getCanvasWidth() * 0.5f;
	}
}

void Player::cameraOffsetY(float multiplier1, float multiplier2)
{
	if (m_pos_y >= m_state->getCanvasHeight() * (multiplier1 - 0.5f) && (m_pos_y <= m_state->getCanvasHeight() * (multiplier2 + 0.5f))) //? prevents going outside of background
	{ 
		if (m_pos_y + m_state->m_global_offset_y <= m_state->getCanvasHeight() * multiplier1)
		{
			m_state->m_global_offset_y = m_state->getCanvasHeight() * multiplier1 - m_pos_y; 
		}
		else if (m_pos_y + m_state->m_global_offset_y >= m_state->getCanvasHeight() * multiplier2)
		{
			m_state->m_global_offset_y = m_state->getCanvasHeight() * multiplier2 - m_pos_y; 
		}
	}
	else if (m_pos_y < m_state->getCanvasHeight() * (multiplier1 - 0.5f))	
	{
		m_state->m_global_offset_y = m_state->getCanvasHeight() * 0.5f;
	}
	else if (m_pos_y > m_state->getCanvasHeight() * (multiplier2 + 0.5f))
	{
		m_state->m_global_offset_y = -m_state->getCanvasHeight() * 0.5f;
	}
}