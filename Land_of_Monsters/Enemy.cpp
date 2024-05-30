#include "Enemy.h"


Enemy::Enemy(Atlas* atlas_enemy_left, Atlas* atlas_enemy_right)
{
	//��ʼ��point
	enemyPosition.x = 0;
	enemyPosition.y = 0;

	//��ʼ��touchbox
	enemyBox.x = enemyPosition.x;
	enemyBox.y = enemyPosition.y;
	enemyBox.w = ENEMY_WIDTH;
	enemyBox.h = ENEMY_HEIGHT;

	//���ص��������ƶ��Ķ���
	enemy_left_animation = new Animation(atlas_enemy_left, 200);

	//���ص��������ƶ��Ķ���
	enemy_right_animation = new Animation(atlas_enemy_right, 200);

	//���λ��
	int birthPosition = rand() % 4;
	switch (birthPosition)
	{
	case UP:
		enemyPosition.x = rand() % getwidth();
		enemyPosition.y = 0 - ENEMY_HEIGHT;
		break;
	case DOWN:
		enemyPosition.x = rand() % getwidth();
		enemyPosition.y = getheight();
		break;
	case LEFT:
		enemyPosition.x = 0 - ENEMY_WIDTH;
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
		bullet->setIsInvalid(true);
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
		enemyPosition.x += (int)(ENEMY_SPEED * dir_x / len_dir);
		enemyPosition.y += (int)(ENEMY_SPEED * dir_y / len_dir);
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

}
