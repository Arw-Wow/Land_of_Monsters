#pragma once

#include "Point.h"

class TouchBox
{
public:
	TouchBox(int _x = 0, int _y = 0, int _w = 0, int _h = 0);
	TouchBox(const TouchBox& other);

	void initSpace();

	//true说明touch，false说明没touch
	bool CheckTouch(TouchBox other);
	bool CheckTouch(int _x, int _y);

	Point getPosition() { return Point(x, y); }

	void changeSize(int change_w, int change_h);	//加上传入的值，若想缩小就传入负值


public:

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

private:

	int space_x = 0;
	int space_y = 0;	// 略微缩小touchbox的范围，毕竟不是一个矩形，碰到边角就算touch不太好
};

