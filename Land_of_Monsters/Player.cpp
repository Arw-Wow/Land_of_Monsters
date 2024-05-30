#include "Player.h"

#include <iostream>

Player::Player(Atlas* atlas_player_left, Atlas* atlas_player_right)
{
	//��ʼ��point
	playerPosition.x = PLAYER_INIT_X;
	playerPosition.y = PLAYER_INIT_Y;

	//��ʼ��touchbox
	playerBox.x = playerPosition.x;
	playerBox.y = playerPosition.y;
	playerBox.w = PLAYER_WIDTH;
	playerBox.h = PLAYER_HEIGHT;

	Boom_box.x = playerPosition.x - BOOM_SIZE / 2 + PLAYER_WIDTH / 2;
	Boom_box.y = playerPosition.y - BOOM_SIZE / 2 + PLAYER_HEIGHT / 2;
	Boom_box.w = BOOM_SIZE;
	Boom_box.h = BOOM_SIZE;

	//�������վ�Ų�����ͼƬ
	loadimage(&player_stand_image, "res/image/player/stand.png", PLAYER_WIDTH, PLAYER_HEIGHT);

	//����Boom��ͼƬ
	loadimage(&skill_Boom_image, "res/image/player/Boom.png", BOOM_SIZE, BOOM_SIZE);

	//������������ƶ��Ķ���
	player_left_animation = new Animation(atlas_player_left, 70);

	//������������ƶ��Ķ���
	player_right_animation = new Animation(atlas_player_right, 70);

}

Player::~Player()
{
	delete player_left_animation;
	delete player_right_animation;
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
		case VK_SPACE:	//Boom��
			isBoom = true;
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

	if (playerPosition.x < 0)							playerPosition.x = 0;
	if (playerPosition.y < 0)							playerPosition.y = 0;
	if (playerPosition.x + PLAYER_WIDTH > getwidth())	playerPosition.x = getwidth() - PLAYER_WIDTH;
	if (playerPosition.y + PLAYER_HEIGHT > getheight())	playerPosition.y = getheight() - PLAYER_HEIGHT;

	x_change = dir_x;
	playerBox.x = playerPosition.x;
	playerBox.y = playerPosition.y;	//����boxλ��

	Boom_box.x = playerPosition.x - BOOM_SIZE / 2 + PLAYER_WIDTH / 2;
	Boom_box.y = playerPosition.y - BOOM_SIZE / 2 + PLAYER_HEIGHT / 2;
}

void Player::beAttacked()
{
	//��ʱ������һ�ξ���
	MessageBox(GetHWnd(), "�����ˣ�", "��Ϸ����", MB_OK);
}


void Player::Draw(int delta)
{

	static bool faceing_left = false;

	if (!is_move)
	{
		putimage(playerPosition.x, playerPosition.y, &player_stand_image);
		return;
	}

	// ���x_changeΪ0������ԭ���ĳ�������ʱ��ı䳯��
	if (x_change < 0)		faceing_left = true;
	else if (x_change > 0)	faceing_left = false;

	if (faceing_left)
		player_left_animation->play(playerPosition, delta);
	else
		player_right_animation->play(playerPosition, delta);
}

void Player::releaseBoom()
{
	putimage(Boom_box.x, Boom_box.y, &skill_Boom_image);
	std::cout << "Boom" << std::endl;
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


