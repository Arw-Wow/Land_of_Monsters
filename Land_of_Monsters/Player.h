#pragma once

#include <graphics.h>
#include <string>
#include "Animation.h"
#include "Point.h"
#include "TouchBox.h"


enum _PLAYER_STATE {
	PLAYER_NORMAL, PLAYER_SUPER, 
};


constexpr int PLAYER_NORMAL_WIDTH = 48;
constexpr int PLAYER_NORMAL_HEIGHT = 80;
constexpr int PLAYER_SUPER_WIDTH = 72;
constexpr int PLAYER_SUPER_HEIGHT = 100;

constexpr int BOOM_NORMAL_DAMAGE = 3;
constexpr int BOOM_SUPER_DAMAGE = 5;
constexpr int BOOM_NORMAL_COST = 1;
constexpr int BOOM_SUPER_COST = 2;


constexpr int PLAYER_INIT_X = 600;
constexpr int PLAYER_INIT_Y = 350;

constexpr int BOOM_NORMAL_SIZE = 250;	//boom的初始大小
constexpr int BOOM_SUPER_SIZE = 500;	//super boom的初始大小

constexpr int PLAYER_SPEED = 4; //人物移动速度


class Player
{
public:

	Player(Atlas* atlas_player_left, Atlas* atlas_player_right, Atlas* atlas_boom);

	~Player();

	//获取、处理事件信息
	void Event(ExMessage& msg);

	void Move();

	int getHp() const { return Hp; }

	void setHp(int hp) { Hp = hp; }
	
	int getMp() const { return Mp; }

	void setMp(int mp) { Mp = mp; }

	void beAttacked(int damage);

	int getDamage() const { return damage; }

	int getBoomDamage() const { return Boom_damage; }

	bool isAlive() const { return Hp > 0; }

	int getState() const { return m_state; }

	void setState(int state) { m_state = state; }

	void Draw(int delta);

	void InitNewState(Atlas* atlas_player_left, Atlas* atlas_player_right, Atlas* atlas_boom);

	// void releaseBoom(int delta);


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
	
	int m_interval = 100;	//动画切换时间间隔

	Animation* boom_animation;
	// int boom_interval;

	// IMAGE player_stand_image;
	// IMAGE skill_Boom_image;
	TouchBox Boom_box;

	int Hp = 3;
	int Mp = 3;
	int damage = 1;
	int Boom_damage = 3;
	int Boom_cost_Mp = 1;

	int m_width = PLAYER_NORMAL_WIDTH;
	int m_height = PLAYER_NORMAL_HEIGHT;
	int boom_size = BOOM_NORMAL_SIZE;

	int no_damage_time = 500; //ms
	DWORD last_damage_time = GetTickCount();

	int m_state = PLAYER_NORMAL;

};

