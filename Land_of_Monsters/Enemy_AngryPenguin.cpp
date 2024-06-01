#include "Enemy_AngryPenguin.h"



Enemy_AngryPenguin::Enemy_AngryPenguin(Atlas* atlas_enemy_angry_penguin_left, Atlas* atlas_enemy_angry_penguin_right) : Enemy(atlas_enemy_angry_penguin_left, atlas_enemy_angry_penguin_right, 45, 50)
{
	Hp = 1;
	damage = 1;
	speed = 2;

	m_interval = 70;

	type = ANGRY_PENGUIN;

	kill_score = 10;
}

