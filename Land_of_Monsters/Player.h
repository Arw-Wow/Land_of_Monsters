#pragma once

#include <graphics.h>
#include <string>
#include "Animation.h"
#include "Point.h"
#include "TouchBox.h"


#define PLAYER_WIDTH 55
#define PLAYER_HEIGHT 55
#define PLAYER_INIT_X 600
#define PLAYER_INIT_Y 350

#define BOOM_SIZE 300

#define PLAYER_SPEED 4 //人物移动速度


class Player
{
public:

	Player(Atlas* atlas_player_left, Atlas* atlas_player_right);

	~Player();

	//获取、处理事件信息
	void Event(ExMessage& msg);

	void Move();

	void beAttacked();

	void Draw(int delta);

	void releaseBoom();


	Point getPosition() const;

	TouchBox getBox() const;

	TouchBox getBoomBox() const;

public:

	bool isBoom = false;


protected:


private:

	Point playerPosition;

	TouchBox playerBox;
	
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;
	bool is_move = false;
	double x_change = 0.0;

	Animation* player_left_animation;
	Animation* player_right_animation;

	IMAGE player_stand_image;
	IMAGE skill_Boom_image;
	TouchBox Boom_box;

};

