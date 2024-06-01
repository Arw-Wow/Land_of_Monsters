/*
�����½ 2.0

player��Hp = 3��Mp = 3�������������ӵ�damage = 1��Boom damage = 3��Boom cost Mp = 1��speed = 4��

angry penguin��Hp = 1��damage = 1��speed = 2��kill score = 10��

evil fanatic��Hp = 3��damage = 1��speed = 3��kill score = 50��

super state: damage = 2, Boom damage = 5, Boom cost Mp = 2;


��ʼ1500ms����һ������
ÿ300�֣����ɼ��ʱ����СΪԭ����0.8��

*/



#include <graphics.h>
#include <iostream>
#include <string>
#include <vector>

#include "Atlas.h"
#include "Animation.h"
#include "Player.h"
#include "Enemy.h"
#include "Point.h"
#include "Button.h"
#include "StartButton.h"
#include "ExitButton.h"
#include "Enemy_AngryPenguin.h"
#include "Enemy_EvilFanatic.h"

#include "Tool.h"



// ����Ϊȫ�ֱ���

Atlas* atlas_player_normal_left;
Atlas* atlas_player_normal_right;

Atlas* atlas_player_super_left;
Atlas* atlas_player_super_right;

Atlas* atlas_normal_boom;
Atlas* atlas_super_boom;

//angry penguin
Atlas* atlas_enemy_angry_penguin_left;
Atlas* atlas_enemy_angry_penguin_right;

// evil fanatic
Atlas* atlas_enemy_evil_fanatic_left;
Atlas* atlas_enemy_evil_fanatic_right;



// ���ӿ�(mciSendString()����winϵͳ����ָ��)
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "MSIMG32.LIB")



constexpr int FPS = 1000 / 100;
constexpr int ADD_MP_INTERVAL = 10000;	//����Mp�ļ�� ms



// ȫ�ֱ���
IMAGE bk_img;
IMAGE start_bk_img;

bool is_start;
int score;

Player* player;

int generate_interval = 1500;	//���ɵ��˵ļ�� ms

int Level = 1;


inline void OutputTitleText()
{
	char outtext[256] = "�����½";
	setbkmode(TRANSPARENT);
	settextcolor(RED);
	settextstyle(150, 0, "��������");
	outtextxy(350, 150, outtext);

	//���ϱ߿�������������ʽ��
	settextcolor(YELLOW);
	settextstyle(150, 0, "���Ĳ���");
	outtextxy(350, 150, outtext);


}

inline void OutputScoreText() {
	char outtext[256] = { 0 };
	sprintf_s(outtext, sizeof(outtext), "��ǰ�÷֣�%d", score);
	setbkmode(TRANSPARENT);
	settextcolor(YELLOW);
	settextstyle(30, 0, "΢���ź�");
	outtextxy(1100, 30, outtext);
}

inline void OutputHpAndMpText()
{
	char outtext[256] = { 0 };
	setbkmode(TRANSPARENT);	//�������屳��Ϊ͸��

	sprintf_s(outtext, sizeof(outtext), "Hp��%d", player->getHp());
	settextcolor(RED);
	settextstyle(30, 0, "΢���ź�");
	outtextxy(10, 30, outtext);

	sprintf_s(outtext, sizeof(outtext), "Mp��%d", player->getMp());
	settextcolor(BLUE);
	settextstyle(30, 0, "΢���ź�");
	outtextxy(10, 70, outtext);

}

inline void OutputLevelText()
{
	char outtext[256] = { 0 };
	setbkmode(TRANSPARENT);	//�������屳��Ϊ͸��

	sprintf_s(outtext, sizeof(outtext), "Level: %d", Level);
	settextcolor(RED);
	settextstyle(45, 0, "��������");
	outtextxy(590, 30, outtext);

}

void LoadSomeImgage() {
	//���ر���
	loadimage(&bk_img, "res/image/bk.jpg", 1280, 720);
	loadimage(&start_bk_img, "res/image/start_bk.jpg", 1280, 720);


	// ����һ�ο�ʼ����
	putimage_alpha(0, 0, &start_bk_img);
}

inline void Draw_Tick(Player* player, std::vector <Enemy* > & enemy_list, std::vector <Bullet* > & bullet_list, DWORD end_time) {
	int delta = GetTickCount() - end_time;
	
	// ���Ʊ���
	putimage_alpha(0, 0, &bk_img);

	// ���Ƶ÷�����
	OutputScoreText();
	// ����Hp
	OutputHpAndMpText();
	// ���Ƶ�ǰ�Ѷȵȼ�
	OutputLevelText();
	
	// ����enemy
	for (int i = 0; i < enemy_list.size(); i++)
	{
		enemy_list[i]->Draw(delta);
	}
	
	// ����player
	player->Draw(delta);

	// �����ӵ�
	for (int i = 0; i < bullet_list.size(); i++)
	{
		bullet_list[i]->Draw();
	}
	
}

int GetRandomTypeOfEnemy()
{
	//��ÿ�����˸�Ȩֵ��ȨֵԽ�߳��ָ���Խ�ߣ���ȨΪ100������ǰ�����壬ȨֵΪ�ۼӶ��Ƕ���
	static const int Weight_of_angry_penguin = 80;
	static const int Weight_of_evil_fanatic = 100;

	int random_num = rand() % 100;	//����0~99�������
	int chose_type = ANGRY_PENGUIN;

	if (random_num < Weight_of_angry_penguin)
	{
		chose_type = ANGRY_PENGUIN;
	}
	else if (random_num < Weight_of_evil_fanatic)
	{
		chose_type = EVIL_FANATIC;
	}

	return chose_type;


}

void TryGenerateEnemy(std::vector <Enemy* > & enemy_list)
{
	static int timer = 1;

	// һ��ʱ������һ������
	if (timer % (generate_interval / FPS) == 0)
	{
		Enemy* enemy;
		int enemy_type = GetRandomTypeOfEnemy();
		switch (enemy_type)
		{
		case ANGRY_PENGUIN:
			enemy = new Enemy_AngryPenguin(atlas_enemy_angry_penguin_left, atlas_enemy_angry_penguin_right);
			break;
		case EVIL_FANATIC:
			enemy = new Enemy_EvilFanatic(atlas_enemy_evil_fanatic_left, atlas_enemy_evil_fanatic_right);
		}
		enemy_list.push_back(enemy);
		timer = 0;
	}
	timer++;
}

void TryAddPlayerMp()
{
	static int timer = 1;

	//һ��ʱ���һ��Mp
	if (timer * FPS % ADD_MP_INTERVAL == 0)
	{
		player->setMp(player->getMp() + 1);
		timer = 0;
	}

	timer++;
}

inline void TrySpeedUp()
{
	if (score / (300 * Level) > 0)
	{
		Level++;
		generate_interval *= 0.8;
	}
}


int main() {
	// ������������ӣ������������λ��
	srand(time(NULL));


	//��ʼ��score
	score = 0;
	is_start = false;


	//��ʼ��atlas
	atlas_player_normal_left = new Atlas(std::string("res/image/player/normal_state/left"), 6, PLAYER_NORMAL_WIDTH, PLAYER_NORMAL_HEIGHT);
	atlas_player_normal_right = new Atlas(std::string("res/image/player/normal_state/right"), 6, PLAYER_NORMAL_WIDTH, PLAYER_NORMAL_HEIGHT);

	atlas_player_super_left = new Atlas(std::string("res/image/player/super_state/left"), 6, PLAYER_SUPER_WIDTH, PLAYER_SUPER_HEIGHT);
	atlas_player_super_right = new Atlas(std::string("res/image/player/super_state/right"), 6, PLAYER_SUPER_WIDTH, PLAYER_SUPER_HEIGHT);

	atlas_normal_boom = new Atlas(std::string("res/image/player/boom/normal_boom"), 3, BOOM_NORMAL_SIZE, BOOM_NORMAL_SIZE);
	atlas_super_boom = new Atlas(std::string("res/image/player/boom/super_boom"), 3, BOOM_SUPER_SIZE, BOOM_SUPER_SIZE);

	atlas_enemy_angry_penguin_left = new Atlas(std::string("res/image/enemy/angry_penguin/left"), 10, 45, 50);
	atlas_enemy_angry_penguin_right = new Atlas(std::string("res/image/enemy/angry_penguin/right"), 10, 45, 50);

	atlas_enemy_evil_fanatic_left = new Atlas(std::string("res/image/enemy/evil_fanatic/left"), 6, 140, 120);
	atlas_enemy_evil_fanatic_right = new Atlas(std::string("res/image/enemy/evil_fanatic/right"), 6, 140, 120);

	//��������
	initgraph(1280, 720);
	// ����һЩͼƬ�������ȣ�
	LoadSomeImgage();


	//��winϵͳ��������������֣���ȡ����Ϊbgm��
	mciSendString(_T("open res/music/bkmusic.mp3 alias bgm"), NULL, 0, NULL);
	//������Ч
	mciSendString(_T("open res/music/shoot.mp3 alias shoot"), NULL, 0, NULL);
	mciSendString(_T("open res/music/enemy_death.mp3 alias enemy_death"), NULL, 0, NULL);
	mciSendString(_T("open res/music/boom.mp3 alias boom"), NULL, 0, NULL);
	mciSendString(_T("open res/music/player_change_state.mp3 alias player_change_state"), NULL, 0, NULL);


	//�������˳غ��ӵ���
	std::vector <Enemy* >  enemy_list;
	std::vector <Bullet* > bullet_list;

	//����start��exit��ť
	StartButton start_button(TouchBox(540, 400, 200, 80), "res/image/button/start_state");
	ExitButton exit_button(TouchBox(540, 530, 200, 80), "res/image/button/exit_state");
	
	ExMessage msg;

	DWORD begin_time = GetTickCount(), end_time = GetTickCount();
	
	BeginBatchDraw();
	
	bool running = true;
	
START:
	// ÿ�ο�ʼ��Ϸ�������µ�Ψһ��Ҷ��󣨳�ʼΪnormal��
	player = new Player(atlas_player_normal_left, atlas_player_normal_right, atlas_normal_boom);

	//�������֣��ظ����ţ�ÿ�δ�ͷ���ţ�
	mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
	
	while (running)
	{
		if (!is_start)
		{
			while (peekmessage(&msg))
			{
				start_button.Event(msg);
				exit_button.Event(msg);
			}

			cleardevice();
			putimage_alpha(0, 0, &start_bk_img);
			start_button.Draw();
			exit_button.Draw();
			OutputTitleText();
			FlushBatchDraw();
			// std::cout << "draw" << std::endl;

			is_start = start_button.IsClicked();

			if (exit_button.IsClicked())	//����˳���ť�Ƴ�����
				goto END;
		}
		else if (is_start)
		{
			begin_time = GetTickCount();

			// ������̺����������Ϣ����while������ÿһ֡���ظ�ִ�л�ȡ��Ϣ������ifÿһֻ֡�ܻ�ȡһ����Ϣ��
			while (peekmessage(&msg))
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					Bullet* bullet = new Bullet(player->getPosition(), Point(msg.x, msg.y));
					bullet_list.push_back(bullet);
					//�����ӵ�������Ч
					mciSendString(_T("play shoot from 0"), NULL, 0, NULL);
				}

				static int last_state = PLAYER_NORMAL;
				
				player->Event(msg);
				
				if (player->getState() == PLAYER_SUPER && last_state == PLAYER_NORMAL)
				{
					player->InitNewState(atlas_player_super_left, atlas_player_super_right, atlas_super_boom);
					mciSendString(_T("play player_change_state from 0"), NULL, 0, NULL);
				}
				else if (player->getState() == PLAYER_NORMAL && last_state == PLAYER_SUPER)
				{
					player->InitNewState(atlas_player_normal_left, atlas_player_normal_right, atlas_normal_boom);
					mciSendString(_T("play player_change_state from 0"), NULL, 0, NULL);
				}
				last_state = player->getState();
			}

			TryGenerateEnemy(enemy_list);
			
			//һ��ʱ���һ��Mp
			TryAddPlayerMp();

			//�����ƶ�
			player->Move();
			for (int i = 0; i < enemy_list.size(); i++)
			{
				enemy_list[i]->Move(player);
			}
			for (int i = 0; i < bullet_list.size(); i++)
			{
				bullet_list[i]->Move();
			}

			//�ж��Ƿ�touch
			for (int i = 0; i < enemy_list.size(); i++)
			{
				//�жϵ��˺���ҵ�touch
				if (enemy_list[i]->CheckPlayerTouch(player))
				{
					player->beAttacked(enemy_list[i]->getDamage());

					//����������
					if (!player->isAlive())
					{

						//��ʾ����HpΪ0
						cleardevice();
						Draw_Tick(player, enemy_list, bullet_list, end_time);
						FlushBatchDraw();

						is_start = false;
						start_button.setClicked(false);
						delete player;

						for (int k = enemy_list.size() - 1; k >= 0; k--)
						{
							delete enemy_list[k];
							enemy_list.pop_back();
						}
						for (int k = bullet_list.size() - 1; k >= 0; k--)
						{
							delete bullet_list[k];
							bullet_list.pop_back();
						}

						//��������
						score = 0;

						MessageBox(GetHWnd(), "�㱻�����ɱ�ˣ�", "��Ϸ����", MB_OK);

						goto START;
					}
				}

				//�жϵ��˺��ӵ���touch
				for (int j = 0; j < bullet_list.size(); j++)
				{
					//������˱��򵽲��Ҳ������޵�֡
					if (enemy_list.size() != 0 && enemy_list[i]->CheckBulletTouch(bullet_list[j]) && enemy_list[i]->beAttacked(player->getDamage()))
					{
						//ɾ���ӵ�
						delete bullet_list[j];
						std::swap(bullet_list[j], bullet_list.back());
						bullet_list.pop_back();

						std::cout << "delete" << std::endl;

						//�����������
						if (!enemy_list[i]->isAlive())
						{
							//ɾ�����ˣ����ӷ�
							score += enemy_list[i]->getScore();

							//���Լ���
							TrySpeedUp();

							delete enemy_list[i];
							std::swap(enemy_list[i], enemy_list.back());
							enemy_list.pop_back();

							//����������Ч
							mciSendString(_T("play enemy_death from 0"), NULL, 0, NULL);


							break;	//��break�ͻ�����⣬��Ϊ��ǰenmey�Ѿ�ɾ�ˣ�������ѭ�����ж���
						}
					}
				}
			
				//�жϵ��˺�Boom��touch
				if (enemy_list.size() != 0 && player->isBoom && enemy_list[i]->CheckBoomTouch(player->getBoomBox()))
				{
					enemy_list[i]->beAttacked(player->getBoomDamage());

					//�����������
					if (!enemy_list[i]->isAlive())
					{
						score += enemy_list[i]->getScore();

						delete enemy_list[i];
						std::swap(enemy_list[i], enemy_list.back());
						enemy_list.pop_back();

						//����������Ч
						mciSendString(_T("play enemy_death from 0"), NULL, 0, NULL);

						continue;
					}
				}
			}

			//���������Ļ���ӵ�
			for (int i = 0; i < bullet_list.size(); i++)
			{
				if (bullet_list[i]->getIsInvalid())
				{
					delete bullet_list[i];
					std::swap(bullet_list[i], bullet_list.back());
					bullet_list.pop_back();

					// std::cout << "delete bullet" << std::endl;	//����Ƿ��Ѿ�ɾ�������߽���ӵ�
				}
			}

			//���Ʋ���
			cleardevice();
			Draw_Tick(player, enemy_list, bullet_list, end_time);
			FlushBatchDraw();

		}
		
		// ��̬����֡��
		end_time = GetTickCount();
		if (end_time - begin_time < FPS) {
			Sleep(FPS - (end_time - begin_time));
		}
	}

	EndBatchDraw();



	//�ͷ���Դ
	delete player;
	for (int i = 0; i < enemy_list.size(); i++)
		delete enemy_list[i];
	for (int i = 0; i < bullet_list.size(); i++)
		delete bullet_list[i];

END:
	delete atlas_player_normal_left;
	delete atlas_player_normal_right;
	delete atlas_player_super_left;
	delete atlas_player_super_right;
	delete atlas_enemy_angry_penguin_left;
	delete atlas_enemy_angry_penguin_right;

	return 0;
}