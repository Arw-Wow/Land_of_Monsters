#include "TouchBox.h"



TouchBox::TouchBox(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h)
{
	space_x = w / 6;
	space_y = h / 6;
}

TouchBox::TouchBox(const TouchBox& other)
{
	x = other.x;
	y = other.y;
	h = other.h;
	w = other.w;
	space_x = other.space_x;
	space_y = other.space_y;
}

void TouchBox::initSpace()
{
	space_x = w / 6;
	space_y = h / 6;
}

bool TouchBox::CheckTouch(TouchBox other)
{
	int m_inner_x = x + space_x;
	int m_inner_w = w - 2 * space_x;
	int m_inner_y = y + space_y;
	int m_inner_h = h - 2 * space_y;	
	
	int o_inner_x = other.x + other.space_x;
	int o_inner_w = other.w - 2 * other.space_x;
	int o_inner_y = other.y + other.space_y;
	int o_inner_h = other.h - 2 * other.space_y;


	if (m_inner_x + m_inner_w > o_inner_x && m_inner_x < o_inner_x + o_inner_w && m_inner_y + m_inner_h > o_inner_y && m_inner_y < o_inner_y + o_inner_h)
		return true;
	return false;
}

bool TouchBox::CheckTouch(int _x, int _y)
{
	if (_x > x && _x < x + w && _y > y && _y < y + h)
		return true;
	return false;
}

void TouchBox::changeSize(int change_w, int change_h)
{
	x += change_w / 2;
	y += change_h / 2;
	w -= change_w;
	h -= change_h;
}


