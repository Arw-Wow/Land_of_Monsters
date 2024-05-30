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


// 设置为全局变量

Atlas* atlas_player_left;
Atlas* atlas_player_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;



// 链接库(mciSendString()，向win系统发送指令)
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "MSIMG32.LIB")



#define FPS 1000 / 100
#define GENERATE_INTERVAL 1000	//生成敌人的间隔 ms



// 背景设为全局变量
IMAGE bk_img;
IMAGE start_bk_img;

bool is_start;
int score;




void OutputScoreText() {
	char outtext[256] = { 0 };
	sprintf_s(outtext, sizeof(outtext), "当前得分：%d", score);
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, "微软雅黑");
	outtextxy(10, 10, outtext);
}

void LoadSomeImgage() {
	//加载背景
	loadimage(&bk_img, "res/image/bk.jpg", 1280, 720);
	loadimage(&start_bk_img, "res/image/start_bk.jpg", 1280, 720);


	// 绘制一次开始界面
	putimage(0, 0, &start_bk_img);
}

void Draw_Tick(Player* player, std::vector <Enemy* > & enemy_list, std::vector <Bullet* > & bullet_list, DWORD end_time) {
	int delta = GetTickCount() - end_time;
	
	// 绘制背景
	putimage(0, 0, &bk_img);
	
	// 绘制player
	player->Draw(delta);

	if (player->isBoom)
	{
		static int BoomCounter = 0;	//Boom的计时器
		BoomCounter++;

		player->releaseBoom();

		if (BoomCounter == 100)
		{
			player->isBoom = false;
			BoomCounter = 0;
		}
	}
	
	// 绘制enemy
	for (int i = 0; i < enemy_list.size(); i++)
	{
		enemy_list[i]->Draw(delta);
	}

	// 绘制子弹
	for (int i = 0; i < bullet_list.size(); i++)
	{
		bullet_list[i]->Draw();
	}

	// 绘制得分文字
	OutputScoreText();

	
}

void OutputTitleText()
{
	char outtext[256] = "怪物大陆";
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(150, 0, "微软雅黑");
	outtextxy(400, 150, outtext);

}


void TryGenerateEnemy(std::vector <Enemy* > & enemy_list) {
	static int counter = 0;
	
	// 一秒生成一个
	if (counter * FPS % GENERATE_INTERVAL == 0)
	{
		Enemy* enemy = new Enemy(atlas_enemy_left, atlas_enemy_right);
		enemy_list.push_back(enemy);
		counter = 0;
	}
	counter++;
}



int main() {
	// 加入随机数种子，随机敌人生成位置
	srand(time(NULL));


	//初始化score
	score = 0;
	is_start = false;


	//初始化atlas
	atlas_player_left = new Atlas(std::string("res/image/player/left"), 12, PLAYER_WIDTH, PLAYER_HEIGHT);
	atlas_player_right = new Atlas(std::string("res/image/player/right"), 12, PLAYER_WIDTH, PLAYER_HEIGHT);
	atlas_enemy_left = new Atlas(std::string("res/image/enemy/left"), 4, ENEMY_WIDTH, ENEMY_HEIGHT);
	atlas_enemy_right = new Atlas(std::string("res/image/enemy/right"), 4, ENEMY_WIDTH, ENEMY_HEIGHT);

	//创建窗口
	initgraph(1280, 720);
	// 加载一些图片（背景等）
	LoadSomeImgage();


	//对win系统发送命令加载音乐（并取别名为bgm）
	mciSendString(_T("open res/music/bkmusic.mp3 alias bgm"), NULL, 0, NULL);
	//加载音效
	mciSendString(_T("open res/music/shoot.mp3 alias shoot"), NULL, 0, NULL);
	mciSendString(_T("open res/music/enemy_death.mp3 alias enemy_death"), NULL, 0, NULL);


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
	// 每次开始游戏都创建新的唯一玩家对象
	Player* player = new Player(atlas_player_left, atlas_player_right);

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
			putimage(0, 0, &start_bk_img);
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

				player->Event(msg);
			}

			TryGenerateEnemy(enemy_list);

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

					//分数清零
					score = 0;

					goto START;
				}

				for (int j = 0; j < bullet_list.size(); j++)
				{
					if (enemy_list[i]->CheckBulletTouch(bullet_list[j]))
					{
						//敌人啊被击中，删除敌人和子弹，并加分
						delete enemy_list[i];
						std::swap(enemy_list[i], enemy_list.back());
						enemy_list.pop_back();

						//播放死亡音效
						mciSendString(_T("play enemy_death from 0"), NULL, 0, NULL);

						delete bullet_list[j];
						std::swap(bullet_list[j], bullet_list.back());
						bullet_list.pop_back();

						score += 10;

						break;	//不break就会出问题，因为当前enmey已经删了，不能再循环来判断了
					}
				}
			
				if (player->isBoom && enemy_list[i]->CheckBoomTouch(player->getBoomBox()))
				{
						//敌人被Boom，删除敌人，并加分
						delete enemy_list[i];
						std::swap(enemy_list[i], enemy_list.back());
						enemy_list.pop_back();

						//播放死亡音效
						mciSendString(_T("play enemy_death from 0"), NULL, 0, NULL);

						score += 10;

						break;
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
	delete atlas_player_left;
	delete atlas_player_right;
	delete atlas_enemy_left;
	delete atlas_enemy_right;

	return 0;
}