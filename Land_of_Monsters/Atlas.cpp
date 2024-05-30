// #include "Atlas.h"
#include "Animation.h"


Atlas::Atlas(std::string path, int num, int width, int height)
{
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

