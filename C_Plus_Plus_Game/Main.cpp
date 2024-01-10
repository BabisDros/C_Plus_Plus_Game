/*
if //!!, really urgent, needs fix
if //?, basic info
if //!, needs improvement
if //??, uncertain
if //, general use, does not fall in above categories
*/
#pragma once
#include "sgg/graphics.h"
#include "util.h"
#include "GameState.h"
#include <string>
#include <stdio.h>
#include <thread>
#include "ParticleManager.h"

void init()
{
	GameState::getInstance()->init();	
}

void draw()
{
	GameState::getInstance()->draw();
}

void update(float dt)
{	
	GameState::getInstance()->update(dt);
}

void updateParticleManager()
{	
	while (true) 
	{
		float dt = graphics::getDeltaTime();
		ParticleManager::getInstance()->threadUpdate(graphics::getDeltaTime());
		std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(6));//needed to run
	}
}
int main(int argc, char** argv)
{
	graphics::createWindow(1200, 600, "Test Stage1");

	init();
	ParticleManager::getInstance()->init();
	
	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);
	
	graphics::setCanvasSize(GameState::getInstance()->getCanvasWidth(), 
							GameState::getInstance()->getCanvasHeight());
	
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
	graphics::setFont("assets\\OpenSans-Regular.ttf");
	std::thread particleThread = std::thread(updateParticleManager);
	graphics::startMessageLoop();
	
	return 0;
}