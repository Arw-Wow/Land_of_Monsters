#pragma once
#include "Button.h"

#include <graphics.h>
#include "TouchBox.h"


class StartButton :
    public Button
{


public:
	StartButton(TouchBox box, LPCTSTR path) : Button(box, path) {}
	~StartButton() = default;


protected:



private:

	TouchBox m_box;
	IMAGE state1_img;
	IMAGE state2_img;
	IMAGE state3_img;
	int state = STATE_1;

	bool is_clicked = false;

};

