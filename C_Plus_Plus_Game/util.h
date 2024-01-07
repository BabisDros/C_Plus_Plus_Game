#pragma once
#include <sgg/graphics.h>
#include <iostream>//TODO: for debugging. remove
#define SETCOLOR(c, r, g, b) {c[0] = r; c[1] = g; c[2] = b;}

static void setCustomBrushProperties(graphics::Brush* brush, float fillOpacity, float outlineOpacity, std::string texture="")
{
	brush->fill_opacity = fillOpacity;
	brush->outline_opacity = outlineOpacity;
	brush->texture = texture;
}

static void debugDraw(float centerX, float centerY, float width, float height,int id=-1)
{
	//box visuals
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(centerX, centerY, width, height, debug_brush);

	
	//text visuals
	std::string idStr = std::to_string(id);//cast to string
	graphics::Brush debug_textBrush;
	SETCOLOR(debug_textBrush.outline_color, 1, 0.1f, 0);
	debug_textBrush.fill_opacity = 1.f;
	debug_textBrush.outline_opacity = 1.0f;
	float centeringValue = idStr.size() / 25.f;//centering offset value for 0.2 size font
	graphics::drawText(centerX- centeringValue, centerY, 0.2f, idStr, debug_textBrush);
}

static float calcCenteringXForTextSize(const size_t& stringSize, const float& targetFontSize)
{
	float value = float(stringSize) * targetFontSize* 0.25f;
	return  value;
}

static float calcCenteringYForTextSize(const float& targetFontSize)
{
	return targetFontSize * 0.35f;
}