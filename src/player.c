#include "grim_fetcher.h"

#define STAMINA_REGEN 10

static void orient_player(t_cell *player, int x, int y);
static void collect_soul(t_ivec2 target);

void move_player(int x, int y)
{
	t_ivec2 target;
	char    symbol;

	if (man.state != ONGOING)
		return;
	orient_player(find_cell_by_symbol(man.map.cells, 'P'), x, y);
	set_ivec2(&target, man.player.pos.x + x, man.player.pos.y + y);
	symbol = man.map.cells[target.y * man.map.size.x + target.x].symbol;
	if (symbol == 'e')
	{
		++man.player.steps;
		man.state = VICTORY;
	}
	else if (symbol == '0' || symbol == 'C')
	{
		if (symbol == 'C')
			collect_soul(target);
		move_symbol(&man.map, man.player.pos, target);
		set_ivec2(&man.player.pos, target.x, target.y);
		++man.player.steps;
	}
	return;
}

static void orient_player(t_cell *player, int x, int y)
{
	if ((x > 0 && !strcmp(player->spr->id, "dog_idle_down_left"))
			|| (y > 0 && !strcmp(player->spr->id, "dog_idle_up_right")))
		player->spr = &man.sprites[13];
	else if ((x < 0 && !strcmp(player->spr->id, "dog_idle_down_right"))
			|| (y > 0 && !strcmp(player->spr->id, "dog_idle_up_left")))
		player->spr = &man.sprites[14];
	else if ((y < 0 && !strcmp(player->spr->id, "dog_idle_down_right"))
			|| (x > 0 && !strcmp(player->spr->id, "dog_idle_up_left")))
		player->spr = &man.sprites[15];
	else if ((y < 0 && !strcmp(player->spr->id, "dog_idle_down_left"))
			|| (x < 0 && !strcmp(player->spr->id, "dog_idle_up_right")))
		player->spr = &man.sprites[16];
	return;
}

static void collect_soul(t_ivec2 target)
{
	t_cell *exit;

	++man.player.collected;
	man.map.cells[target.y * man.map.size.x + target.x].spr = 0;
	man.player.stamina = f_clamp(man.player.stamina + STAMINA_REGEN, 0, 100);
	if (man.player.collected == man.player.to_collect)
	{
		exit = find_cell_by_symbol(man.map.cells, 'E');
		exit->symbol = 'e';
		exit->spr = get_spr_by_symbol(exit->symbol);
	}
	return;
}
