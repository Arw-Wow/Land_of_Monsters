#include "TouchBox.h"

bool TouchBox::CheckTouch(TouchBox other)
{
	if (x + w > other.x && x < other.x + other.w && y + h > other.y && y < other.y + other.h)
		return true;
	return false;
}

bool TouchBox::CheckTouch(int _x, int _y)
{
	if (_x > x && _x < x + w && _y > y && _y < y + h)
		return true;
	return false;
}


