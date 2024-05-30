#pragma once
class Point
{
public:

	Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
	Point operator = (const Point& other);


public:

	int x;
	int y;

};

