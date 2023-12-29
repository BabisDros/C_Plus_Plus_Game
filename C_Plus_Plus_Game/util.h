#pragma once
#include <sgg/graphics.h>
#define SETCOLOR(c, r, g, b) {c[0] = r; c[1] = g; c[2] = b;}

static void setCustomBrushProperties(graphics::Brush* brush, float fill, float outline, std::string texture)
{
	brush->fill_opacity = 1.0f;
	brush->outline_opacity = 0.0f;
	brush->texture = texture;
}

static void debugDraw(float centerX, float centerY, float width, float height)
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;

	graphics::drawRect(centerX, centerY, width, height, debug_brush);
}