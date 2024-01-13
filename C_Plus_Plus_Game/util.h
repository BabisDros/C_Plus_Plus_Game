#pragma once
#include <sgg/graphics.h>
#include <iostream>//TODO: for debugging. remove
#define SETCOLOR(c, r, g, b) {c[0] = r; c[1] = g; c[2] = b;}

static void setCustomBrushProperties(graphics::Brush* brush, float fillOpacity, float outlineOpacity, std::string texture = "");
static void debugDraw(float centerX, float centerY, float width, float height, int id = -1);
static float calcCenteringXForTextSize(const std::string str, const float& targetFontSize);
static float calcCenteringYForTextSize(const float& targetFontSize);

static void setCustomBrushProperties(graphics::Brush* brush, float fillOpacity, float outlineOpacity, std::string texture)
{
	brush->fill_opacity = fillOpacity;
	brush->outline_opacity = outlineOpacity;
	brush->texture = texture;
}


static void debugDraw(float centerX, float centerY, float width, float height,int id)
{
	//box visuals
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(centerX, centerY, width, height, debug_brush);

	
	//text visuals with calculated offset based on this Font MONACO
	std::string idStr = std::to_string(id);//cast to string
	graphics::Brush debug_textBrush;
	SETCOLOR(debug_textBrush.outline_color, 1, 0.1f, 0);
	debug_textBrush.fill_opacity = 1.f;
	debug_textBrush.outline_opacity = 1.0f;
	float fontSize = 0.2f;
	float centeringValueX = calcCenteringXForTextSize(idStr, fontSize);
	float centeringValueY= calcCenteringYForTextSize(fontSize);
	graphics::drawText(centerX - centeringValueX, centerY+centeringValueY, fontSize, idStr, debug_textBrush);
}

/*in order to work correctly, use monospace font MONACO because the space size 0.18 is calculated specificly
param stringSize : e.g m_healthTxt. To understand how centering works see debugDraw method*/
static float calcCenteringXForTextSize(const std::string str, const float& targetFontSize)
{
	int spaceCount = 0;
	for (char c : str) 
	{
		if (c == ' ') 
		{
			spaceCount++;
		}
	}

	float stringSize = str.size();
	float value = (float(stringSize) *  0.25f - spaceCount * 0.18f)* targetFontSize;
	return  value;
}
/*To understand how centering works see debugDraw method*/
static float calcCenteringYForTextSize(const float& targetFontSize)
{
	return targetFontSize * 0.35f;
}