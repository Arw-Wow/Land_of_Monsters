#pragma once

#include <string>
#include <vector>
#include <graphics.h>


// 共享数据类（目前存有animation）
class Atlas
{
public:

	Atlas(std::string path, int num, int width = 70, int height = 70);

	~Atlas();

	void resetSize(int w, int h);

public:

	std::vector < IMAGE * > frame_list;

	std::string m_path;
	int m_num;

};

