#include "Bullet.h"
#include <corecrt_math.h>

#define BULLET_SIZE 15

Bullet::Bullet(Point _pos, Point destination)
{
	m_pos = _pos;
	m_pos.x += 55 / 2 - BULLET_SIZE / 2;
	m_pos.y += 55 / 2 - BULLET_SIZE / 2;	//为了让子弹出现在人物中间而不是左上角

	// 计算出每次移动的x和y变化量
	int dir_x = destination.x - m_pos.x;
	int dir_y = destination.y - m_pos.y;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	x_change = int(m_speed * dir_x / len_dir);
	y_change = int(m_speed * dir_y / len_dir);

	loadimage(&m_image, "res/image/bullet/bullet.png", BULLET_SIZE, BULLET_SIZE);
}

Bullet::~Bullet()
{

}

void Bullet::Move()
{

	m_pos.x += x_change;
	m_pos.y += y_change;

	m_box.x = m_pos.x;
	m_box.y = m_pos.y;

	// 超出边界就失效
	if (m_pos.x < 0 || m_pos.x + BULLET_SIZE > getwidth() || m_pos.y < 0 || m_pos.y + BULLET_SIZE > getheight())
		is_invalid = true;

}

void Bullet::Draw()
{
	putimage(m_pos.x, m_pos.y, &m_image);
}
