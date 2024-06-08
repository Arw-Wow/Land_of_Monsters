#include "Enemy.h"


Enemy::Enemy(Atlas* atlas_enemy_left, Atlas* atlas_enemy_right, int width, int height)
{
	m_interval = 70;
	m_width = width;
	m_height = height;

	//��ʼ��point
	enemyPosition.x = 0;
	enemyPosition.y = 0;

	//��ʼ��touchbox
	enemyBox.x = enemyPosition.x;
	enemyBox.y = enemyPosition.y;
	enemyBox.w = m_width;
	enemyBox.h = m_height;
	enemyBox.initSpace();

	//���ص��������ƶ��Ķ���
	enemy_left_animation = new Animation(atlas_enemy_left, m_interval);

	//���ص��������ƶ��Ķ���
	enemy_right_animation = new Animation(atlas_enemy_right, m_interval);

	//���λ��
	// int birthPosition = rand() % 4;
	int birthPosition = rand() % 3 + 1;	//��Ҫ����ճ���
	switch (birthPosition)
	{
	case UP:
		enemyPosition.x = rand() % getwidth();
		enemyPosition.y = 0 - m_height;
		break;
	case DOWN:
		enemyPosition.x = rand() % getwidth();
		enemyPosition.y = getheight();
		break;
	case LEFT:
		enemyPosition.x = 0 - m_width;
		enemyPosition.y = rand() % getheight();
		break;
	case RIGHT:
		enemyPosition.x = getwidth();
		enemyPosition.y = rand() % getheight();
		break;
	}
	enemyBox.x = enemyPosition.x;
	enemyBox.y = enemyPosition.y;

	std::cout << enemyPosition.x << " " << enemyPosition.y << std::endl;
}

Enemy::~Enemy()
{
	delete enemy_left_animation;
	delete enemy_right_animation;
}

bool Enemy::CheckBulletTouch(Bullet* bullet)
{
	if (enemyBox.CheckTouch(bullet->getBox()))
	{
		return true;
	}
	return false;
}

bool Enemy::CheckPlayerTouch(Player* player)
{
	if (enemyBox.CheckTouch(player->getBox()))
	{
		return true;
	}
	return false;
}

bool Enemy::CheckBoomTouch(TouchBox Boom_box)
{
	if (enemyBox.CheckTouch(Boom_box))
	{
		return true;
	}
	return false;
}

void Enemy::Move(const Player* player)
{
	Point playerPosition = player->getPosition();

	// ͨ��һ������֪ʶ������֤��ɫ������б���ƶ���ʱ���Ϊ���Ŷ�����
	int dir_x = playerPosition.x - enemyPosition.x;
	int dir_y = playerPosition.y - enemyPosition.y;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);

	if (len_dir != 0)
	{
		enemyPosition.x += (int)(speed * dir_x / len_dir);
		enemyPosition.y += (int)(speed * dir_y / len_dir);
	}
	// len_dir����0˵���Ѿ��������λ�ã��Ͳ���

	// ����0�Ͳ��ı䳯��
	if (dir_x != 0)
		x_change = dir_x;

	//enemy���ᳬ����ͼ�����Բ���Ҫ��Щ�ж�
	// if (enemyPosition.x < 0)							enemyPosition.x = 0;
	// if (enemyPosition.y < 0)							enemyPosition.y = 0;
	// if (enemyPosition.x + PLAYER_WIDTH > getwidth())	enemyPosition.x = getwidth() - ENEMY_WIDTH;
	// if (enemyPosition.y + PLAYER_HEIGHT > getheight())	enemyPosition.y = getheight() - ENEMY_HEIGHT;

	enemyBox.x = enemyPosition.x;
	enemyBox.y = enemyPosition.y;	// ����boxλ��

}

void Enemy::Draw(int delta)
{

	if (x_change < 0)
		enemy_left_animation->play(enemyPosition, delta);
	else
		enemy_right_animation->play(enemyPosition, delta);

	int hp_grid_length = 20;

	int hp_width = hp_grid_length * Hp;
	int hp_height = m_height / 8;

	int offset_x = (m_width - hp_width) / 2;

	setfillcolor(RED);
	fillrectangle(enemyPosition.x + offset_x, enemyPosition.y + hp_height, enemyPosition.x + offset_x + hp_width, enemyPosition.y);

}

bool Enemy::beAttacked(int damage)
{
	//����ϴ��ܻ�����εļ��С��500ms���Ͳ���Ѫ���޵�500ms��
	if ((int)(GetTickCount() - last_damage_time) < (int)no_damage_time)
		return false;

	Hp -= damage;

	last_damage_time = GetTickCount();

	return true;
}
