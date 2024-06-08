#include "Player.h"

#include "Tool.h"

#include <iostream>
#include "Bullet.h"

Player::Player(Atlas* atlas_player_left, Atlas* atlas_player_right, Atlas* atlas_boom)
{
	//��ʼ��point
	playerPosition.x = PLAYER_INIT_X;
	playerPosition.y = PLAYER_INIT_Y;

	//��ʼ��touchbox
	playerBox.x = playerPosition.x;
	playerBox.y = playerPosition.y;
	playerBox.w = PLAYER_NORMAL_WIDTH;
	playerBox.h = PLAYER_NORMAL_HEIGHT;
	playerBox.initSpace();

	Boom_box.x = playerPosition.x - BOOM_NORMAL_SIZE / 2 + PLAYER_NORMAL_WIDTH / 2;
	Boom_box.y = playerPosition.y - BOOM_NORMAL_SIZE / 2 + PLAYER_NORMAL_HEIGHT / 2;
	Boom_box.w = BOOM_NORMAL_SIZE;
	Boom_box.h = BOOM_NORMAL_SIZE;
	Boom_box.initSpace();

	//����Boom�Ķ���
	boom_animation = new Animation(atlas_boom, 130);

	//������������ƶ��Ķ���
	player_left_animation = new Animation(atlas_player_left, m_interval);

	//������������ƶ��Ķ���
	player_right_animation = new Animation(atlas_player_right, m_interval);

}

Player::~Player()
{
	delete player_left_animation;
	delete player_right_animation;

	delete boom_animation;
}

//��ȡ�������¼���Ϣ
void Player::Event(ExMessage& msg) 
{
	//���̰��£������ƶ�
	if (msg.message == WM_KEYDOWN) {
		switch (msg.vkcode)
		{
		case 0x57:	//W ���������
			is_move_up = true;
			break;
		case 0x53:	//S
			is_move_down = true;
			break;
		case 0x41:	//A
			is_move_left = true;
			break;
		case 0x44:	//D
			is_move_right = true;
			break;
		case VK_SPACE:	//Boom��
			if (Mp >= Boom_cost_Mp && isBoom == false)
			{
				Mp -= Boom_cost_Mp;
				isBoom = true;
				mciSendString(_T("play boom from 0"), NULL, 0, NULL);
			}
			break;
		}
	}
	else if (msg.message == WM_KEYUP) {
		switch (msg.vkcode)
		{
		case 0x57:
			is_move_up = false;
			break;
		case 0x53:
			is_move_down = false;
			break;
		case 0x41:
			is_move_left = false;
			break;
		case 0x44:
			is_move_right = false;
			break;
		case 0x46:	//F���л���̬���ſ�F�Ŵ�����
			if (m_state == PLAYER_NORMAL)
			{
				m_state = PLAYER_SUPER;
			}
			else if (m_state == PLAYER_SUPER)
			{
				m_state = PLAYER_NORMAL;
			}
			break;
		}
	}
}

void Player::Move()
{
	// ͨ��һ������֪ʶ������֤��ɫ������б���ƶ���ʱ���Ϊ���Ŷ�����
	int dir_x = is_move_right - is_move_left;
	int dir_y = is_move_down - is_move_up;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	double x_fenliang = 0, y_fenliang = 0;

	if (len_dir != 0)
	{
		is_move = true;
		x_fenliang = dir_x / len_dir;
		y_fenliang = dir_y / len_dir;
		playerPosition.x += (int)(PLAYER_SPEED * x_fenliang);
		playerPosition.y += (int)(PLAYER_SPEED * y_fenliang);
	}
	else
	{
		is_move = false;
	}

	if (playerPosition.x < 0)									playerPosition.x = 0;
	if (playerPosition.y < 0)									playerPosition.y = 0;
	if (playerPosition.x + m_width > getwidth())	playerPosition.x = getwidth() - m_width;
	if (playerPosition.y + m_height > getheight())	playerPosition.y = getheight() - m_height;

	x_change = dir_x;
	playerBox.x = playerPosition.x;
	playerBox.y = playerPosition.y;	//����boxλ��

	Boom_box.x = playerPosition.x - boom_size / 2 + m_width / 2;
	Boom_box.y = playerPosition.y - boom_size / 2 + m_height / 2;
}

void Player::beAttacked(int damage)
{
	// super״̬�޵�
	// if (m_state == PLAYER_SUPER)
	//	return;

	//����ϴ��ܻ�����εļ��С��500ms���Ͳ���Ѫ���޵�500ms��
	if ((int)(GetTickCount() - last_damage_time) < (int)no_damage_time)
		return;

	Hp -= damage;

	mciSendString("play hurt from 0", NULL, 0, NULL);

	last_damage_time = GetTickCount();
}

void Player::Draw(int delta)
{

	static bool faceing_left = false;

	// ���x_changeΪ0������ԭ���ĳ�������ʱ��ı䳯��
	if (x_change < 0)		faceing_left = true;
	else if (x_change > 0)	faceing_left = false;

	if (faceing_left)
		player_left_animation->play(playerPosition, delta);
	else
		player_right_animation->play(playerPosition, delta);


	if (isBoom)
	{
		static int counter = 0;
		if (counter == 140)
		{
			counter = 0;
			isBoom = false;
			boom_animation->reload();
			return;
		}
		boom_animation->play(Boom_box.getPosition(), delta);

		counter++;
	}
}

void Player::InitNewState(Atlas* atlas_player_left, Atlas* atlas_player_right, Atlas* atlas_boom)
{
	if (m_state == PLAYER_NORMAL)
	{
		m_width = PLAYER_NORMAL_WIDTH;
		m_height = PLAYER_NORMAL_HEIGHT;
		damage = 1;
		boom_size = BOOM_NORMAL_SIZE;
		Boom_damage = BOOM_NORMAL_DAMAGE;
		Boom_cost_Mp = BOOM_NORMAL_COST;
		Bullet::setState(BULLET_NORMAL);
	}
	else if (m_state == PLAYER_SUPER)
	{
		m_width = PLAYER_SUPER_WIDTH;
		m_height = PLAYER_SUPER_HEIGHT;
		damage = 2;
		boom_size = BOOM_SUPER_SIZE;
		Boom_damage = BOOM_SUPER_DAMAGE;
		Boom_cost_Mp = BOOM_SUPER_COST;
		Bullet::setState(BULLET_SUPER);
	}

	playerBox.w = m_width;
	playerBox.h = m_height;
	playerBox.initSpace();

	Boom_box.x = playerPosition.x - boom_size / 2 + m_width / 2;
	Boom_box.y = playerPosition.y - boom_size / 2 + m_height / 2;
	Boom_box.w = boom_size;
	Boom_box.h = boom_size;
	Boom_box.initSpace();

	//����Boom�Ķ���
	boom_animation->setAnimation(atlas_boom, 130);

	//������������ƶ��Ķ���
	player_left_animation->setAnimation(atlas_player_left, m_interval);

	//������������ƶ��Ķ���
	player_right_animation->setAnimation(atlas_player_right, m_interval);

}

Point Player::getPosition() const
{
	return playerPosition;
}

TouchBox Player::getBox() const
{
	return playerBox;
}

TouchBox Player::getBoomBox() const
{
	return Boom_box;
}



// void Player::releaseBoom(int delta)
// {

//	// putimage_alpha(Boom_box.x, Boom_box.y, &skill_Boom_image);
//	// std::cout << "Boom" << std::endl;
// }

