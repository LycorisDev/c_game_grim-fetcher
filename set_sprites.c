#include "grim_fetcher.h"

int				create_background(t_win *win);
int				set_xpm_file_obj(t_xpm *file, int fd);
int				get_color_bpp(t_img *img, size_t i, int is_shadow);
int				create_sprites_from_file(t_win *win, t_xpm *file, int *i_spr);

static int		set_pixel_data(t_win *win, t_xpm *file, int *i_spr);
static t_uint	*get_pixel_data(t_win *win, t_xpm *file, int is_shadow);

int	set_sprite_array(t_win *win, char *path)
{
	int		fd;
	int		i_spr;
	int		is_success;
	int		is_parsing_ongoing;
	t_xpm	file;

	if (!create_background(win))
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	i_spr = 1;
	is_success = 1;
	is_parsing_ongoing = 1;
	ft_bzero(&file, sizeof(t_xpm));
	while (is_parsing_ongoing && is_success)
	{
		is_parsing_ongoing = set_xpm_file_obj(&file, fd);
		if (is_parsing_ongoing)
			is_success = set_pixel_data(win, &file, &i_spr);
		free_and_reset_xpm_data(&file);
	}
	close(fd);
	get_next_line(-1);
	return (is_success);
}

// For xpm_file.c
char	*findc(char *s, char c)
{
	size_t	i;

	if (!c)
		return (s + ft_strlen(s));
	i = 0;
	while (s[i] && ft_isspace(s[i]))
		++i;
	if (c < 0)
		return (s + i);
	while (s[i])
	{
		if (s[i] == c)
			return (s + i);
		++i;
	}
	return (0);
}

// For xpm_file.c
void	allocate_segment_cycle(t_xpm_seg *seg)
{
	int	i;

	seg->cycle = malloc(seg->cycle_len * sizeof(t_ivec2));
	if (seg->cycle)
	{
		i = 0;
		while (i < seg->cycle_len)
			set_ivec2(&seg->cycle[i++], 0, 0);
	}
	return ;
}

static int	set_pixel_data(t_win *win, t_xpm *file, int *i_spr)
{
	file->data = get_pixel_data(win, file, 0);
	if (file->path_shadow)
		file->data_shadow = get_pixel_data(win, file, 1);
	if (!file->data || (file->path_shadow && !file->data_shadow)
		|| !create_sprites_from_file(win, file, i_spr))
		return (0);
	return (1);
}

static t_uint	*get_pixel_data(t_win *win, t_xpm *file, int is_shadow)
{
	size_t	i;
	size_t	data_len;
	char	*path;
	t_img	*img;
	t_uint	*data;

	path = file->path;
	if (is_shadow)
		path = file->path_shadow;
	img = mlx_xpm_file_to_image(win->mlx, path, &file->size.x, &file->size.y);
	if (!img)
		return (0);
	data = malloc(img->width * img->height * sizeof(t_uint));
	if (data)
	{
		i = 0;
		data_len = img->width * img->height;
		while (i < data_len)
		{
			data[i] = get_color_bpp(img, i, is_shadow);
			++i;
		}
	}
	mlx_destroy_image(win->mlx, img);
	return (data);
}
