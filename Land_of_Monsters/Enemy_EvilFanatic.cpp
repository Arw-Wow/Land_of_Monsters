#include "Enemy_EvilFanatic.h"

Enemy_EvilFanatic::Enemy_EvilFanatic(Atlas* atlas_enemy_angry_penguin_left, Atlas* atlas_enemy_angry_penguin_right) : Enemy(atlas_enemy_angry_penguin_left, atlas_enemy_angry_penguin_right, 140, 120)
{
	Hp = 3;
	damage = 1;
	speed = 3;

	m_interval = 70;

	type = EVIL_FANATIC;

	kill_score = 50;
}


