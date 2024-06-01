#include "Animation.h"

#include "Tool.h"

Animation::Animation(Atlas* atlas, int interval)
{
	m_atlas = atlas;
	m_interval = interval;
}

void Animation::play(Point Position, int delta)
{
	//ÿ��һ��ʱ�䣬�͸�Ϊ������һ֡
	timer += delta;
	if (timer >= m_interval) {
		frame_index++;
		frame_index %= m_atlas->frame_list.size();
		timer -= m_interval;
	}

	putimage_alpha(Position.x, Position.y, m_atlas->frame_list[frame_index]);

}

void Animation::setAnimation(Atlas* atlas, int interval)
{
	m_atlas = atlas;
	m_interval = interval;
}

void Animation::reload()
{
	frame_index = 0;
}

