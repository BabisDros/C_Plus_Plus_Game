/*
if //!!, really urgent, needs fix
if //?, basic info
if //!, needs improvement
if //??, uncertain
if //, general use, does not fall in above categories
*/

#include "sgg/graphics.h"
#include "util.h"
#include "GameState.h"
#include <string>
#include <stdio.h>

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

	graphics::startMessageLoop();
	return 0;
}