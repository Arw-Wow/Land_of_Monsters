#pragma once

#include <string>
#include <vector>
#include <graphics.h>


// ���������ࣨĿǰ����animation��
class Atlas
{
public:

	Atlas(std::string path, int num, int width = 70, int height = 70);

	~Atlas();

public:

	std::vector < IMAGE * > frame_list;

};

