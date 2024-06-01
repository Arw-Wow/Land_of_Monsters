#pragma once

#include <graphics.h>
#include "Point.h"
#include "TouchBox.h"



enum _BULLET_STATE {
	BULLET_NORMAL, BULLET_SUPER,
};



constexpr int BULLET_NORMAL_SIZE = 30;

constexpr int BULLET_SUPER_SIZE = 50;

// 子弹类
class Bullet
{
public:

	Bullet(Point _pos, Point destination);
	
	~Bullet();

	void Move();

	void Draw();

	// 如果invalid就在main里delete
	bool getIsInvalid() const { return is_invalid; }
	void setIsInvalid(bool in) { is_invalid = in; }

	// bool getIsPlaySound() const { return is_play_sound; }
	// void setIsPlaySound(bool in) { is_play_sound = in; }

	TouchBox getBox() const { return m_box; }

	static void setState(int state) { bullet_state = state; }

protected:



private:

	int m_speed = 10;
	Point m_pos;
	int x_change;
	int y_change;
	IMAGE m_image;
	TouchBox m_box;

	int m_size = BULLET_NORMAL_SIZE;

	bool is_invalid = false;

	static int bullet_state;

	// bool is_play_sound = false;

};

