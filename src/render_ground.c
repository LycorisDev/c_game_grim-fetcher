#include "grim_fetcher.h"

void	draw_ground(t_win *win, t_frame *frame, t_ivec2 pos)
{
	draw_sprite(frame, &win->sprites[9], pos, 0);
	set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
	draw_sprite(frame, &win->sprites[9], pos, 0);
	set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
	draw_sprite(frame, &win->sprites[9], pos, 0);
	set_ivec2(&pos, pos.x - win->sprites[9].size.x * 2,
		pos.y + win->sprites[9].size.y);
	draw_sprite(frame, &win->sprites[9], pos, 0);
	set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
	draw_sprite(frame, &win->sprites[9], pos, 0);
	set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
	draw_sprite(frame, &win->sprites[9], pos, 0);
	set_ivec2(&pos, pos.x - win->sprites[9].size.x * 2,
		pos.y + win->sprites[9].size.y);
	draw_sprite(frame, &win->sprites[9], pos, 0);
	set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
	draw_sprite(frame, &win->sprites[9], pos, 0);
	set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
	draw_sprite(frame, &win->sprites[9], pos, 0);
	return ;
}
