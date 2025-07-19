#include "grim_fetcher.h"

static void draw_background(t_color *buf, int width, int height, int seed);
static void set_color(t_color *buf, int i, float rng, t_color color);

int create_background(void)
{
	t_spr *s;
	int   i;

	s = &man.sprites[0];
	memset(s, 0, sizeof(t_spr));
	s->id = strdup("background");
	if (!s->id)
		return 0;
	s->size.x = RES_WIDTH;
	s->size.y = RES_HEIGHT;
	s->cycle_len = 1;
	s->cycle = malloc(s->cycle_len * sizeof(int *));
	if (!s->cycle)
		return 0;
	memset(s->cycle, 0, s->cycle_len * sizeof(int *));
	s->cycle[0] = malloc(s->size.x * s->size.y * sizeof(int));
	if (!s->cycle[0])
		return 0;
	memset(s->cycle[0], 0, s->size.x * s->size.y * sizeof(int));
	i = 0;
	while (i < s->size.x * s->size.y)
		s->cycle[0][i++].a = 255;
	draw_background(s->cycle[0], s->size.x, s->size.y, 4);
	return 1;
}

void render_background(t_frame *frame)
{
	t_ivec2 pos;

	set_ivec2(&pos, 0, 0);
	draw_sprite(frame, &man.sprites[0], pos, 0);
	return;
}

static void draw_background(t_color *buf, int width, int height, int seed)
{
	t_ivec2 p;
	t_ivec2 c;
	float   max_dist;
	float   factor;

	c.x = width / 2;
	c.y = height / 2;
	max_dist = f_dist_euclidean(c.x, c.y, 0, 0);
	p.y = 0;
	while (p.y < height)
	{
		p.x = 0;
		while (p.x < width)
		{
			factor = f_sqrt(f_dist_euclidean(p.x, p.y, c.x, c.y) / max_dist);
			set_color(buf,
					p.y * width + p.x,
					rng_minmax(&seed, 0, width) / (float)width,
					get_color_rgba(108 * (1 - factor), 0, 255 * (1 - factor), 255));
			++p.x;
		}
		++p.y;
	}
	return;
}

static void set_color(t_color *buf, int i, float rng, t_color color)
{
	t_color sky;
	t_color star;

	if (rng < 0.001f)
	{
		sky.a = 0xFF;
		sky.r = color.r;
		sky.g = color.g;
		sky.b = color.b;
		star.a = 0x7F;
		star.r = 0xE3;
		star.g = 0xDE;
		star.b = 0xD1;
		color = get_alpha_blended_color(sky, star);
		color.a = 255;
		buf[i] = color;
	}
	else if (rng < 0.002f)
		buf[i] = get_color_rgba(255, 255, 255, 255);
	else
		buf[i] = color;
	return;
}
