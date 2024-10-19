#include "grim_fetcher.h"

static void	draw_background(t_uint *buf, int width, int height, int seed);
static void	set_color(t_uint *buf, int i, float rng, t_uint color);

int	create_background(t_win *win)
{
	t_spr	*s;
	int		i;

	s = &win->sprites[0];
	ft_bzero(s, sizeof(t_spr));
	s->id = ft_strdup("background");
	if (!s->id)
		return (0);
	s->size.x = RES_WIDTH;
	s->size.y = RES_HEIGHT;
	s->cycle_len = 1;
	s->cycle = malloc(s->cycle_len * sizeof(int *));
	if (!s->cycle)
		return (0);
	ft_bzero(s->cycle, s->cycle_len * sizeof(int *));
	s->cycle[0] = malloc(s->size.x * s->size.y * sizeof(int));
	if (!s->cycle[0])
		return (0);
	ft_bzero(s->cycle[0], s->size.x * s->size.y * sizeof(int));
	i = 0;
	while (i < s->size.x * s->size.y)
		set_alpha(&s->cycle[0][i++], 255);
	draw_background(s->cycle[0], s->size.x, s->size.y, 4);
	return (1);
}

void	render_background(t_win *win, t_frame *frame)
{
	t_ivec2	pos;

	set_ivec2(&pos, 0, 0);
	draw_sprite(frame, &win->sprites[0], pos, 0);
	return ;
}

static void	draw_background(t_uint *buf, int width, int height, int seed)
{
	t_ivec2	p;
	t_ivec2	c;
	float	max_dist;
	float	factor;

	c.x = width / 2;
	c.y = height / 2;
	max_dist = get_dist_euclidean(c.x, c.y, 0, 0);
	p.y = 0;
	while (p.y < height)
	{
		p.x = 0;
		while (p.x < width)
		{
			factor = ft_sqrt(get_dist_euclidean(p.x, p.y, c.x, c.y) / max_dist);
			set_color(
				buf,
				p.y * width + p.x,
				rng_minmax(&seed, 0, width) / (float)width,
				get_color(255, 128 * (1 - factor), 0, 255 * (1 - factor)));
			++p.x;
		}
		++p.y;
	}
	return ;
}

static void	set_color(t_uint *buf, int i, float rng, t_uint color)
{
	t_ubyte	sky[4];
	t_ubyte	star[4];

	if (rng < 0.001f)
	{
		sky[0] = 0xFF;
		sky[1] = get_red(color);
		sky[2] = get_green(color);
		sky[3] = get_blue(color);
		star[0] = 0x7F;
		star[1] = 0xE3;
		star[2] = 0xDE;
		star[3] = 0xD1;
		color = get_alpha_blended_color(sky, star);
		set_alpha(&color, 255);
		buf[i] = color;
	}
	else if (rng < 0.002f)
		buf[i] = 0xFFFFFFFF;
	else
		buf[i] = color;
	return ;
}
