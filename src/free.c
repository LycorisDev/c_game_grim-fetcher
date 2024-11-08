#include "grim_fetcher.h"

void	free_and_reset_xpm_data(t_xpm *file)
{
	int	i;

	free(file->path);
	free(file->path_shadow);
	free(file->data);
	free(file->data_shadow);
	i = 0;
	while (i < file->segment_len)
	{
		free(file->seg[i].id);
		free(file->seg[i].cycle);
		++i;
	}
	free(file->seg);
	ft_bzero(file, sizeof(t_xpm));
	return ;
}

void	free_sprites(t_win *win)
{
	int	i;
	int	j;

	i = 0;
	while (i < SPRITE_LEN)
	{
		free(win->sprites[i].id);
		j = 0;
		while (j < win->sprites[i].cycle_len)
		{
			if (win->sprites[i].cycle)
				free(win->sprites[i].cycle[j]);
			if (win->sprites[i].cycle_shadow)
				free(win->sprites[i].cycle_shadow[j]);
			++j;
		}
		free(win->sprites[i].cycle);
		free(win->sprites[i].cycle_shadow);
		++i;
	}
	return ;
}

void	free_ber_data(char **ber)
{
	char	**ptr;

	ptr = ber;
	while (*ber)
	{
		free(*ber);
		++ber;
	}
	free(ptr);
	return ;
}

void	release_resources(t_win *win)
{
	if (win->win)
		mlx_destroy_window(win->mlx, win->win);
	if (win->frame[0].img)
		mlx_destroy_image(win->mlx, win->frame[0].img);
	if (win->frame[1].img)
		mlx_destroy_image(win->mlx, win->frame[1].img);
	if (win->frame[2].img)
		mlx_destroy_image(win->mlx, win->frame[2].img);
	mlx_destroy_display(win->mlx);
	free(win->mlx);
	free_sprites(win);
	free(win->map.name);
	free(win->map.cells);
	lst_clear(&win->clicked_path, 0);
	return ;
}
