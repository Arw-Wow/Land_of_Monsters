// #include "Atlas.h"
#include "Animation.h"


Atlas::Atlas(std::string path, int num, int width, int height)
{
	m_path = path;
	m_num = num;

	for (int i = 0; i < num; i++)
	{
		std::string s = path + std::to_string(i) + ".png";

		IMAGE* frame = new IMAGE;
		loadimage(frame, s.c_str(), width, height);

		frame_list.push_back(frame);
	}

}

Atlas::~Atlas()
{
	for (int i = 0; i < frame_list.size(); i++)
		delete frame_list[i];
}

void Atlas::resetSize(int w, int h)
{
	for (int i = 0; i < frame_list.size(); i++)
		delete frame_list[i];
	frame_list.clear();

	for (int i = 0; i < m_num; i++)
	{
		std::string s = m_path + std::to_string(i) + ".png";

		IMAGE* frame = new IMAGE;
		loadimage(frame, s.c_str(), w, h);

		frame_list.push_back(frame);
	}

}

