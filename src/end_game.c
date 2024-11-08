#include "grim_fetcher.h"

void	game_over_screen(t_win *win, int victory)
{
	long	dt;
	t_ivec2	pos;
	t_frame	*frame;

	dt = get_delta_time();
	frame = &win->frame[win->curr_frame];
	render_background(win, frame);
	render_gui(win, frame, dt);
	set_ivec2(&pos, RES_WIDTH / 2, RES_HEIGHT / 2);
	if (victory)
	{
		pos.x -= ft_strlen("You won! :D") / 2 * 8;
		draw_font_default(win, frame, &pos, "You won! :D");
	}
	else
	{
		pos.x -= ft_strlen("You got killed by a slime :(") / 2 * 7;
		draw_font_default(win, frame, &pos, "You got killed by a slime :(");
	}
	on_mouse_move(win);
	win->curr_frame = (win->curr_frame + 1) % 2;
	mlx_put_image_to_window(win->mlx, win->win, frame->img, 0, 0);
	return ;
}
