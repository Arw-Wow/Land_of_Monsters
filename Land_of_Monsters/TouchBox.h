#pragma once


class TouchBox
{
public:
	TouchBox() = default;
	TouchBox(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) { }

	//true说明touch，false说明没touch
	bool CheckTouch(TouchBox other);
	bool CheckTouch(int _x, int _y);

	

public:

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};

