#include "Point.h"

Point Point::operator=(const Point& other)
{
	this->x = other.x;
	this->y = other.y;

	return *this;
}
