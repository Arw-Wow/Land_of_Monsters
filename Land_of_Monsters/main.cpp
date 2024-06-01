/*
怪物大陆 2.0

player：Hp = 3，Mp = 3（可增长），子弹damage = 1，Boom damage = 3，Boom cost Mp = 1，speed = 4；

angry penguin：Hp = 1，damage = 1，speed = 2，kill score = 10；

evil fanatic：Hp = 3，damage = 1，speed = 3，kill score = 50；

super state: damage = 2, Boom damage = 5, Boom cost Mp = 2;


初始1500ms生成一个敌人
每300分，生成间隔时间缩小为原来的0.8倍

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



// 设置为全局变量

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



// 链接库(mciSendString()，向win系统发送指令)
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "MSIMG32.LIB")



constexpr int FPS = 1000 / 100;
constexpr int ADD_MP_INTERVAL = 10000;	//增加Mp的间隔 ms



// 全局变量
IMAGE bk_img;
IMAGE start_bk_img;

bool is_start;
int score;

Player* player;

int generate_interval = 1500;	//生成敌人的间隔 ms

int Level = 1;


inline void OutputTitleText()
{
	char outtext[256] = "怪物大陆";
	setbkmode(TRANSPARENT);
	settextcolor(RED);
	settextstyle(150, 0, "华文琥珀");
	outtextxy(350, 150, outtext);

	//镶上边框（利用了字体样式）
	settextcolor(YELLOW);
	settextstyle(150, 0, "华文彩云");
	outtextxy(350, 150, outtext);


}

inline void OutputScoreText() {
	char outtext[256] = { 0 };
	sprintf_s(outtext, sizeof(outtext), "当前得分：%d", score);
	setbkmode(TRANSPARENT);
	settextcolor(YELLOW);
	settextstyle(30, 0, "微软雅黑");
	outtextxy(1100, 30, outtext);
}

inline void OutputHpAndMpText()
{
	char outtext[256] = { 0 };
	setbkmode(TRANSPARENT);	//设置字体背景为透明

	sprintf_s(outtext, sizeof(outtext), "Hp：%d", player->getHp());
	settextcolor(RED);
	settextstyle(30, 0, "微软雅黑");
	outtextxy(10, 30, outtext);

	sprintf_s(outtext, sizeof(outtext), "Mp：%d", player->getMp());
	settextcolor(BLUE);
	settextstyle(30, 0, "微软雅黑");
	outtextxy(10, 70, outtext);

}

inline void OutputLevelText()
{
	char outtext[256] = { 0 };
	setbkmode(TRANSPARENT);	//设置字体背景为透明

	sprintf_s(outtext, sizeof(outtext), "Level: %d", Level);
	settextcolor(RED);
	settextstyle(45, 0, "华文琥珀");
	outtextxy(590, 30, outtext);

}

void LoadSomeImgage() {
	//加载背景
	loadimage(&bk_img, "res/image/bk.jpg", 1280, 720);
	loadimage(&start_bk_img, "res/image/start_bk.jpg", 1280, 720);


	// 绘制一次开始界面
	putimage_alpha(0, 0, &start_bk_img);
}

inline void Draw_Tick(Player* player, std::vector <Enemy* > & enemy_list, std::vector <Bullet* > & bullet_list, DWORD end_time) {
	int delta = GetTickCount() - end_time;
	
	// 绘制背景
	putimage_alpha(0, 0, &bk_img);

	// 绘制得分文字
	OutputScoreText();
	// 绘制Hp
	OutputHpAndMpText();
	// 绘制当前难度等级
	OutputLevelText();
	
	// 绘制enemy
	for (int i = 0; i < enemy_list.size(); i++)
	{
		enemy_list[i]->Draw(delta);
	}
	
	// 绘制player
	player->Draw(delta);

	// 绘制子弹
	for (int i = 0; i < bullet_list.size(); i++)
	{
		bullet_list[i]->Draw();
	}
	
}

int GetRandomTypeOfEnemy()
{
	//给每个敌人赋权值，权值越高出现概率越高，总权为100，按从前到后定义，权值为累加而非独立
	static const int Weight_of_angry_penguin = 80;
	static const int Weight_of_evil_fanatic = 100;

	int random_num = rand() % 100;	//产生0~99的随机数
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

	// 一段时间生成一个敌人
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

	//一段时间加一点Mp
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
	// 加入随机数种子，随机敌人生成位置
	srand(time(NULL));


	//初始化score
	score = 0;
	is_start = false;


	//初始化atlas
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

	//创建窗口
	initgraph(1280, 720);
	// 加载一些图片（背景等）
	LoadSomeImgage();


	//对win系统发送命令加载音乐（并取别名为bgm）
	mciSendString(_T("open res/music/bkmusic.mp3 alias bgm"), NULL, 0, NULL);
	//加载音效
	mciSendString(_T("open res/music/shoot.mp3 alias shoot"), NULL, 0, NULL);
	mciSendString(_T("open res/music/enemy_death.mp3 alias enemy_death"), NULL, 0, NULL);
	mciSendString(_T("open res/music/boom.mp3 alias boom"), NULL, 0, NULL);
	mciSendString(_T("open res/music/player_change_state.mp3 alias player_change_state"), NULL, 0, NULL);


	//创建敌人池和子弹池
	std::vector <Enemy* >  enemy_list;
	std::vector <Bullet* > bullet_list;

	//创建start和exit按钮
	StartButton start_button(TouchBox(540, 400, 200, 80), "res/image/button/start_state");
	ExitButton exit_button(TouchBox(540, 530, 200, 80), "res/image/button/exit_state");
	
	ExMessage msg;

	DWORD begin_time = GetTickCount(), end_time = GetTickCount();
	
	BeginBatchDraw();
	
	bool running = true;
	
START:
	// 每次开始游戏都创建新的唯一玩家对象（初始为normal）
	player = new Player(atlas_player_normal_left, atlas_player_normal_right, atlas_normal_boom);

	//播放音乐（重复播放，每次从头播放）
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

			if (exit_button.IsClicked())	//点击退出按钮推出程序
				goto END;
		}
		else if (is_start)
		{
			begin_time = GetTickCount();

			// 处理键盘和鼠标输入信息（用while可以让每一帧都重复执行获取信息，而用if每一帧只能获取一次信息）
			while (peekmessage(&msg))
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					Bullet* bullet = new Bullet(player->getPosition(), Point(msg.x, msg.y));
					bullet_list.push_back(bullet);
					//创建子弹播放音效
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
			
			//一段时间加一次Mp
			TryAddPlayerMp();

			//进行移动
			player->Move();
			for (int i = 0; i < enemy_list.size(); i++)
			{
				enemy_list[i]->Move(player);
			}
			for (int i = 0; i < bullet_list.size(); i++)
			{
				bullet_list[i]->Move();
			}

			//判断是否touch
			for (int i = 0; i < enemy_list.size(); i++)
			{
				//判断敌人和玩家的touch
				if (enemy_list[i]->CheckPlayerTouch(player))
				{
					player->beAttacked(enemy_list[i]->getDamage());

					//如果玩家死亡
					if (!player->isAlive())
					{

						//显示最后的Hp为0
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

						//分数清零
						score = 0;

						MessageBox(GetHWnd(), "你被怪物击杀了！", "游戏结束", MB_OK);

						goto START;
					}
				}

				//判断敌人和子弹的touch
				for (int j = 0; j < bullet_list.size(); j++)
				{
					//如果敌人被打到并且不处于无敌帧
					if (enemy_list.size() != 0 && enemy_list[i]->CheckBulletTouch(bullet_list[j]) && enemy_list[i]->beAttacked(player->getDamage()))
					{
						//删除子弹
						delete bullet_list[j];
						std::swap(bullet_list[j], bullet_list.back());
						bullet_list.pop_back();

						std::cout << "delete" << std::endl;

						//如果敌人死亡
						if (!enemy_list[i]->isAlive())
						{
							//删除敌人，并加分
							score += enemy_list[i]->getScore();

							//尝试加速
							TrySpeedUp();

							delete enemy_list[i];
							std::swap(enemy_list[i], enemy_list.back());
							enemy_list.pop_back();

							//播放死亡音效
							mciSendString(_T("play enemy_death from 0"), NULL, 0, NULL);


							break;	//不break就会出问题，因为当前enmey已经删了，不能再循环来判断了
						}
					}
				}
			
				//判断敌人和Boom的touch
				if (enemy_list.size() != 0 && player->isBoom && enemy_list[i]->CheckBoomTouch(player->getBoomBox()))
				{
					enemy_list[i]->beAttacked(player->getBoomDamage());

					//如果敌人死亡
					if (!enemy_list[i]->isAlive())
					{
						score += enemy_list[i]->getScore();

						delete enemy_list[i];
						std::swap(enemy_list[i], enemy_list.back());
						enemy_list.pop_back();

						//播放死亡音效
						mciSendString(_T("play enemy_death from 0"), NULL, 0, NULL);

						continue;
					}
				}
			}

			//清除超出屏幕的子弹
			for (int i = 0; i < bullet_list.size(); i++)
			{
				if (bullet_list[i]->getIsInvalid())
				{
					delete bullet_list[i];
					std::swap(bullet_list[i], bullet_list.back());
					bullet_list.pop_back();

					// std::cout << "delete bullet" << std::endl;	//检测是否已经删掉超出边界的子弹
				}
			}

			//绘制步骤
			cleardevice();
			Draw_Tick(player, enemy_list, bullet_list, end_time);
			FlushBatchDraw();

		}
		
		// 动态控制帧率
		end_time = GetTickCount();
		if (end_time - begin_time < FPS) {
			Sleep(FPS - (end_time - begin_time));
		}
	}

	EndBatchDraw();



	//释放资源
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