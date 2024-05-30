#pragma once

#include <graphics.h>
#include "TouchBox.h"


enum _STATE {
	STATE_1, STATE_2, STATE_3, 
};


class Button
{
public:
	Button(TouchBox box, LPCTSTR path);
	~Button() = default;

	void Event(ExMessage& msg);

	void Draw();

	bool IsClicked() { return is_clicked; }
	void setClicked(bool in) { is_clicked = in; }
	


protected:



private:

	TouchBox m_box;
	IMAGE state1_img;
	IMAGE state2_img;
	IMAGE state3_img;
	int state = STATE_1;

	bool is_clicked = false;

};

