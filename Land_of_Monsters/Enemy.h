#pragma once

#include <graphics.h>
#include <string>
#include <iostream>
#include "Animation.h"
#include "Bullet.h"
#include "Player.h"
#include "Point.h"
#include "TouchBox.h"

enum _ENEMY_BRITH_POSITION {
	UP, DOWN, LEFT, RIGHT,
};

enum _ENEMY_TYPE {
	ANGRY_PENGUIN, EVIL_FANATIC,
	ENEMY_AMOUNT	//最后一个是种类数量
};



// 默认是 angry penguin
class Enemy
{
public:

	Enemy() = default;

	Enemy(Atlas* atlas_enemy_left, Atlas* atlas_enemy_right, int width, int height);

	virtual ~Enemy();

	bool CheckBulletTouch(Bullet* bullet);

	bool CheckPlayerTouch(Player* player);

	bool CheckBoomTouch(TouchBox Boom_box);

	void Move(const Player* player);

	void Draw(int delta);

	bool isAlive() const { return Hp > 0; }

	bool beAttacked(int damage);

	int getDamage() const { return damage; }

	int getType() const { return type; }

	int getScore() const { return kill_score; }

protected:

	int m_width = 45;
	int m_height = 50;

	int Hp = 1;

	int damage = 1;

	Point enemyPosition;

	TouchBox enemyBox;

	int x_change = 0;	//小于0说明向左移动，大于零说明向右移动，等于零不变
	

	Animation* enemy_left_animation;
	Animation* enemy_right_animation;

	int m_interval = 70;

	int speed = 2;

	int type = ANGRY_PENGUIN;

	int kill_score = 10;

	int no_damage_time = 500; //ms
	DWORD last_damage_time = GetTickCount();

};

