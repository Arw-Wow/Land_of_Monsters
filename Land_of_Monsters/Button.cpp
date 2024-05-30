#include "Button.h"
#include <string>

Button::Button(TouchBox box, LPCTSTR path)
{
	m_box = box;
	state = STATE_1;
	
	std::string s1 = path + std::to_string(1) + ".png";
	std::string s2 = path + std::to_string(2) + ".png";
	std::string s3 = path + std::to_string(3) + ".png";

	loadimage(&state1_img, s1.c_str(), m_box.w, m_box.h);
	loadimage(&state2_img, s2.c_str(), m_box.w, m_box.h);
	loadimage(&state3_img, s3.c_str(), m_box.w, m_box.h);

}

void Button::Event(ExMessage& msg)
{
	if (!m_box.CheckTouch(msg.x, msg.y))
		state = STATE_1;
	else
	{
		if (state == STATE_1)
			state = STATE_2;

		if (msg.message == WM_LBUTTONDOWN)
			state = STATE_3;

		else if (state == STATE_3 && msg.message == WM_LBUTTONUP)
			setClicked(true);
	}
}

void Button::Draw()
{
	switch (state)
	{
	case STATE_1:
		putimage(m_box.x, m_box.y, &state1_img);
		break;
	case STATE_2:
		putimage(m_box.x, m_box.y, &state2_img);
		break;
	case STATE_3:
		putimage(m_box.x, m_box.y, &state3_img);
		break;
	}
}

