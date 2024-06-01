#include "Bullet.h"
#include <corecrt_math.h>

#include "Tool.h"

int Bullet::bullet_state = BULLET_NORMAL;

Bullet::Bullet(Point _pos, Point destination)
{
	m_pos = _pos;

	if (bullet_state == BULLET_NORMAL)
	{
		m_size = BULLET_NORMAL_SIZE;
		m_pos.x += 60 / 2 - m_size / 2;
		m_pos.y += 80 / 3 - m_size / 2;	//为了让子弹出现在人物中间而不是左上角
		loadimage(&m_image, "res/image/bullet/normal_bullet.png", m_size, m_size);
	}
	else if (bullet_state == BULLET_SUPER)
	{
		m_size = BULLET_SUPER_SIZE;
		m_pos.x += 60 / 2 - m_size / 2;
		m_pos.y += 80 / 3 - m_size / 2;	//为了让子弹出现在人物中间而不是左上角
		loadimage(&m_image, "res/image/bullet/super_bullet.png", m_size, m_size);
	}

	m_box = TouchBox(m_pos.x, m_pos.y, m_size, m_size);

	// 计算出每次移动的x和y变化量
	int dir_x = destination.x - m_pos.x;
	int dir_y = destination.y - m_pos.y;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	x_change = int(m_speed * dir_x / len_dir);
	y_change = int(m_speed * dir_y / len_dir);

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
	if (m_pos.x < 0 || m_pos.x + m_size > getwidth() || m_pos.y < 0 || m_pos.y + m_size > getheight())
		is_invalid = true;

}

void Bullet::Draw()
{
	putimage_alpha(m_pos.x, m_pos.y, &m_image);
}