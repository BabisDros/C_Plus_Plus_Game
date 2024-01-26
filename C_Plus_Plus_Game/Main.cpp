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

int main(int argc, char** argv)
{
	graphics::createWindow(1200, 600, "Test Stage1");

	init();
	
	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);

	graphics::setCanvasSize(GameState::getInstance()->getCanvasWidth(), 
							GameState::getInstance()->getCanvasHeight());
	
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
	/*this spesific font is used because it is monospace and all letters have the same size.
	Is is use so that util methods calcCenteringOffset works correctly*/
	graphics::setFont("assets\\MONACO.ttf");
	
	graphics::startMessageLoop();
	GameState::getInstance()->~GameState();
	return 0;
}