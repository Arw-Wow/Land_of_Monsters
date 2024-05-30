#pragma once

#include <graphics.h>
#include <string>
#include <iostream>
#include "Animation.h"
#include "Bullet.h"
#include "Player.h"
#include "Point.h"
#include "TouchBox.h"


#define ENEMY_WIDTH 40
#define ENEMY_HEIGHT 40

#define ENEMY_SPEED 2 //�����ƶ��ٶ�

enum _ENEMY_BRITH_POSITION {
	UP, DOWN, LEFT, RIGHT, 
};

class Enemy
{
public:

	Enemy(Atlas* atlas_enemy_left, Atlas* atlas_enemy_right);

	~Enemy();

	bool CheckBulletTouch(Bullet* bullet);

	bool CheckPlayerTouch(Player* player);

	bool CheckBoomTouch(TouchBox Boom_box);

	void Move(const Player* player);

	void Draw(int delta);

protected:


private:


	Point enemyPosition;

	TouchBox enemyBox;

	int x_change = 0;	//С��0˵�������ƶ���������˵�������ƶ��������㲻��
	

	Animation* enemy_left_animation;
	Animation* enemy_right_animation;

};

