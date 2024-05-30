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


// ����Ϊȫ�ֱ���

Atlas* atlas_player_left;
Atlas* atlas_player_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;



// ���ӿ�(mciSendString()����winϵͳ����ָ��)
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "MSIMG32.LIB")



#define FPS 1000 / 100
#define GENERATE_INTERVAL 1000	//���ɵ��˵ļ�� ms



// ������Ϊȫ�ֱ���
IMAGE bk_img;
IMAGE start_bk_img;

bool is_start;
int score;




void OutputScoreText() {
	char outtext[256] = { 0 };
	sprintf_s(outtext, sizeof(outtext), "��ǰ�÷֣�%d", score);
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, "΢���ź�");
	outtextxy(10, 10, outtext);
}

void LoadSomeImgage() {
	//���ر���
	loadimage(&bk_img, "res/image/bk.jpg", 1280, 720);
	loadimage(&start_bk_img, "res/image/start_bk.jpg", 1280, 720);


	// ����һ�ο�ʼ����
	putimage(0, 0, &start_bk_img);
}

void Draw_Tick(Player* player, std::vector <Enemy* > & enemy_list, std::vector <Bullet* > & bullet_list, DWORD end_time) {
	int delta = GetTickCount() - end_time;
	
	// ���Ʊ���
	putimage(0, 0, &bk_img);
	
	// ����player
	player->Draw(delta);

	if (player->isBoom)
	{
		static int BoomCounter = 0;	//Boom�ļ�ʱ��
		BoomCounter++;

		player->releaseBoom();

		if (BoomCounter == 100)
		{
			player->isBoom = false;
			BoomCounter = 0;
		}
	}
	
	// ����enemy
	for (int i = 0; i < enemy_list.size(); i++)
	{
		enemy_list[i]->Draw(delta);
	}

	// �����ӵ�
	for (int i = 0; i < bullet_list.size(); i++)
	{
		bullet_list[i]->Draw();
	}

	// ���Ƶ÷�����
	OutputScoreText();

	
}

void OutputTitleText()
{
	char outtext[256] = "�����½";
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(150, 0, "΢���ź�");
	outtextxy(400, 150, outtext);

}


void TryGenerateEnemy(std::vector <Enemy* > & enemy_list) {
	static int counter = 0;
	
	// һ������һ��
	if (counter * FPS % GENERATE_INTERVAL == 0)
	{
		Enemy* enemy = new Enemy(atlas_enemy_left, atlas_enemy_right);
		enemy_list.push_back(enemy);
		counter = 0;
	}
	counter++;
}



int main() {
	// ������������ӣ������������λ��
	srand(time(NULL));


	//��ʼ��score
	score = 0;
	is_start = false;


	//��ʼ��atlas
	atlas_player_left = new Atlas(std::string("res/image/player/left"), 12, PLAYER_WIDTH, PLAYER_HEIGHT);
	atlas_player_right = new Atlas(std::string("res/image/player/right"), 12, PLAYER_WIDTH, PLAYER_HEIGHT);
	atlas_enemy_left = new Atlas(std::string("res/image/enemy/left"), 4, ENEMY_WIDTH, ENEMY_HEIGHT);
	atlas_enemy_right = new Atlas(std::string("res/image/enemy/right"), 4, ENEMY_WIDTH, ENEMY_HEIGHT);

	//��������
	initgraph(1280, 720);
	// ����һЩͼƬ�������ȣ�
	LoadSomeImgage();


	//��winϵͳ��������������֣���ȡ����Ϊbgm��
	mciSendString(_T("open res/music/bkmusic.mp3 alias bgm"), NULL, 0, NULL);
	//������Ч
	mciSendString(_T("open res/music/shoot.mp3 alias shoot"), NULL, 0, NULL);
	mciSendString(_T("open res/music/enemy_death.mp3 alias enemy_death"), NULL, 0, NULL);


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
	// ÿ�ο�ʼ��Ϸ�������µ�Ψһ��Ҷ���
	Player* player = new Player(atlas_player_left, atlas_player_right);

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
			putimage(0, 0, &start_bk_img);
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

				player->Event(msg);
			}

			TryGenerateEnemy(enemy_list);

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
				if (enemy_list[i]->CheckPlayerTouch(player))
				{
					player->beAttacked();
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

					goto START;
				}

				for (int j = 0; j < bullet_list.size(); j++)
				{
					if (enemy_list[i]->CheckBulletTouch(bullet_list[j]))
					{
						//���˰������У�ɾ�����˺��ӵ������ӷ�
						delete enemy_list[i];
						std::swap(enemy_list[i], enemy_list.back());
						enemy_list.pop_back();

						//����������Ч
						mciSendString(_T("play enemy_death from 0"), NULL, 0, NULL);

						delete bullet_list[j];
						std::swap(bullet_list[j], bullet_list.back());
						bullet_list.pop_back();

						score += 10;

						break;	//��break�ͻ�����⣬��Ϊ��ǰenmey�Ѿ�ɾ�ˣ�������ѭ�����ж���
					}
				}
			
				if (player->isBoom && enemy_list[i]->CheckBoomTouch(player->getBoomBox()))
				{
						//���˱�Boom��ɾ�����ˣ����ӷ�
						delete enemy_list[i];
						std::swap(enemy_list[i], enemy_list.back());
						enemy_list.pop_back();

						//����������Ч
						mciSendString(_T("play enemy_death from 0"), NULL, 0, NULL);

						score += 10;

						break;
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
	delete atlas_player_left;
	delete atlas_player_right;
	delete atlas_enemy_left;
	delete atlas_enemy_right;

	return 0;
}