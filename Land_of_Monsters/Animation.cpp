#include "Animation.h"


Animation::Animation(Atlas* atlas, int interval)
{
	m_atlas = atlas;
	m_interval = interval;

}

void Animation::play(Point Position, int delta)
{
	//每过一段时间，就改为播放下一帧
	timer += delta;
	if (timer >= m_interval) {
		frame_index++;
		frame_index %= m_atlas->frame_list.size();
		timer -= m_interval;
	}

	putimage(Position.x, Position.y, m_atlas->frame_list[frame_index]);

}

