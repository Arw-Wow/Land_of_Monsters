#pragma once

#include "Point.h"

class TouchBox
{
public:
	TouchBox(int _x = 0, int _y = 0, int _w = 0, int _h = 0);
	TouchBox(const TouchBox& other);

	void initSpace();

	//true˵��touch��false˵��ûtouch
	bool CheckTouch(TouchBox other);
	bool CheckTouch(int _x, int _y);

	Point getPosition() { return Point(x, y); }

	void changeSize(int change_w, int change_h);	//���ϴ����ֵ��������С�ʹ��븺ֵ


public:

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

private:

	int space_x = 0;
	int space_y = 0;	// ��΢��Сtouchbox�ķ�Χ���Ͼ�����һ�����Σ������߽Ǿ���touch��̫��
};

