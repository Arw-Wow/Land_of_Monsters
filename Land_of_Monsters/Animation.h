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
	
	int m_interval = 0;	//ÿ֡ʱ����
	int timer = 0;	//������ʱ��
	int frame_index = 0;	//����֡����

	Atlas* m_atlas;	//���湲������(Animation)

};
