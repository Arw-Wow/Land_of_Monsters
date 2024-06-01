#pragma once

#include <string>
#include <vector>
#include <graphics.h>
#include "Point.h"
#include "Atlas.h"


class Animation
{
public:

	Animation(Atlas* atlas, int interval);

	~Animation() = default;

	void play(Point Position, int delta);

	void setAnimation(Atlas* atlas, int interval);

	void reload();

protected:

private:
	
	int m_interval = 0;	//每帧时间间隔
	int timer = 0;	//动画计时器
	int frame_index = 0;	//动画帧索引

	Atlas* m_atlas;	//保存共享数据(Animation)

};
