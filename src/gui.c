#include "grim_fetcher.h"

static void	render_steps(t_win *win, t_frame *frame);
static void	render_souls(t_win *win, t_frame *frame, long dt);

void	render_gui(t_win *win, t_frame *frame, long dt)
{
	render_steps(win, frame);
	render_souls(win, frame, dt);
	return ;
}

/*
static void	render_health(t_win *win, t_frame *frame)
{
	t_ivec2	pos;
	char	*text;

	set_ivec2(&pos, 8, 0);
	draw_font_default(win, frame, &pos, " Health: ");
	text = ft_itoa(win->p.health);
	draw_font_default(win, frame, &pos, text);
	free(text);
	draw_font_default(win, frame, &pos, "%");
	return ;
}

static void	render_stamina(t_win *win, t_frame *frame)
{
	t_ivec2	pos;
	char	*text;

	set_ivec2(&pos, 0, 24);
	draw_font_default(win, frame, &pos, "Stamina: ");
	text = ft_itoa(win->p.stamina);
	set_ivec2(&pos, 70, 24);
	draw_font_default(win, frame, &pos, text);
	free(text);
	pos.y = 24;
	draw_font_default(win, frame, &pos, "%");
	return ;
}
*/

static void	render_steps(t_win *win, t_frame *frame)
{
	t_ivec2	pos;
	char	*text;

	set_ivec2(&pos, 300, 0);
	draw_font_default(win, frame, &pos, "Steps: ");
	text = ft_itoa(win->p.steps);
	draw_font_default(win, frame, &pos, text);
	free(text);
	return ;
}

static void	render_souls(t_win *win, t_frame *frame, long dt)
{
	t_ivec2	pos;
	char	*text;

	set_ivec2(&pos, 560, -4);
	draw_sprite(frame, &win->sprites[12], pos, dt);
	set_ivec2(&pos, 594, 0);
	text = ft_itoa(win->p.collected);
	draw_font_default(win, frame, &pos, text);
	free(text);
	draw_font_default(win, frame, &pos, "/");
	text = ft_itoa(win->p.to_collect);
	draw_font_default(win, frame, &pos, text);
	free(text);
	return ;
}
