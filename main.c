#include "grim_fetcher.h"

static int	init_frame(t_win *win, int index);
static void	set_hooks(t_win *win);

int	main(int argc, char **argv)
{
	t_win	win;

	ft_bzero(&win, sizeof(t_win));
	win.mlx = mlx_init();
	if (!win.mlx)
		return (1);
	if (!set_sprite_array(&win, "textures/index.json")
		|| !set_map_and_player(&win, argc, argv[1])
		|| !create_window(&win, RES_WIDTH, RES_HEIGHT))
	{
		ft_dprintf(2, "Error: Failure during initialization\n");
		release_resources(&win);
		return (1);
	}
	add_outline_to_font(&win.sprites[2]);
	mlx_loop(win.mlx);
	return (0);
}

/*
	I had to remove `mlx_mouse_hide(win->mlx, win->win)` because it was 
	causing a leak.
*/
int	create_window(t_win *win, int width, int height)
{
	set_ivec2(&win->size, width, height);
	win->win = mlx_new_window(win->mlx, win->size.x, win->size.y, "Grim Fetcher");
	if (!win->win)
		return (0);
	if (!init_frame(win, 0) || !init_frame(win, 1) || !init_frame(win, 2))
		return (0);
	if (!win->zoom)
		win->zoom = 5;
	set_hooks(win);
	return (1);
}

static int	init_frame(t_win *win, int index)
{
	t_frame	*frame;

	frame = &win->frame[index];
	set_ivec2(&frame->size, win->size.x, win->size.y);
	frame->thickness = frame->size.x / RES_WIDTH;
	frame->img = mlx_new_image(win->mlx, frame->size.x, frame->size.y);
	if (!frame->img)
		return (0);
	frame->addr = mlx_get_data_addr(frame->img, &frame->bpp,
			&frame->line_length, &frame->endian);
	return (1);
}

static void	set_hooks(t_win *win)
{
	mlx_hook(win->win, 17, 0L, on_close, win);
	mlx_hook(win->win, 4, 1L << 2, on_mouse_button, win);
	mlx_key_hook(win->win, on_key_press, win);
	mlx_loop_hook(win->mlx, render_loop, win);
	return ;
}
