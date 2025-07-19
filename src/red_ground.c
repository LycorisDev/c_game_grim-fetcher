#include "grim_fetcher.h"

static void copy_spr_data(const char *id, t_spr *dst, t_spr *src);

void add_red_version_of_ground(void)
{
	int i;
	int len;

	copy_spr_data("red_ground", &man.sprites[63], &man.sprites[9]);
	len = man.sprites[63].size.x * man.sprites[63].size.y;
	i = 0;
	while (i < len)
	{
		man.sprites[63].cycle[0][i].r = 255;
		++i;
	}
	return;
}

int is_cell_a_slime_neighbor(t_ivec2 i)
{
	t_ivec2 size;
	t_cell  *cells;

	set_ivec2(&size, man.map.size.x, man.map.size.y);
	cells = man.map.cells;
	if (cells[i.y * size.x + i.x].symbol == 'E'
			|| cells[i.y * size.x + i.x].symbol == 'e')
		return 0;
	else if (i.y > 0 && (cells[(i.y - 1) * size.x + i.x].symbol == 'B'
				|| cells[(i.y - 1) * size.x + i.x].symbol == 'G'))
		return 1;
	else if (i.y < size.y && (cells[(i.y + 1) * size.x + i.x].symbol == 'B'
				|| cells[(i.y + 1) * size.x + i.x].symbol == 'G'))
		return 1;
	else if (i.x > 0 && (cells[i.y * size.x + (i.x - 1)].symbol == 'B'
				|| cells[i.y * size.x + (i.x - 1)].symbol == 'G'))
		return 1;
	else if (i.x < size.x && (cells[i.y * size.x + (i.x + 1)].symbol == 'B'
				|| cells[i.y * size.x + (i.x + 1)].symbol == 'G'))
		return 1;
	return 0;
}

static void copy_spr_data(const char *id, t_spr *dst, t_spr *src)
{
	int i;
	int j;
	int len;

	len = src->size.x * src->size.y;
	dst->id = strdup(id);
	dst->size.x = src->size.x;
	dst->size.y = src->size.y;
	dst->shadow_offset.x = src->shadow_offset.x;
	dst->shadow_offset.y = src->shadow_offset.y;
	dst->still_frame = src->still_frame;
	dst->cycle_len = src->cycle_len;
	dst->cycle_index = src->cycle_index;
	dst->elapsed_time_in_ms = src->elapsed_time_in_ms;
	dst->cycle = calloc(src->cycle_len, sizeof(t_color *));
	i = 0;
	while (i < src->cycle_len)
	{
		dst->cycle[i] = calloc(len, sizeof(t_color));
		j = 0;
		while (j < len)
		{
			dst->cycle[i][j].r = src->cycle[i][j].r;
			dst->cycle[i][j].g = src->cycle[i][j].g;
			dst->cycle[i][j].b = src->cycle[i][j].b;
			dst->cycle[i][j].a = src->cycle[i][j].a;
			++j;
		}
		++i;
	}
	dst->cycle_shadow = 0;
	if (!src->cycle_shadow)
		return;
	dst->cycle_shadow = calloc(src->cycle_len, sizeof(t_color *));
	i = 0;
	while (i < src->cycle_len)
	{
		dst->cycle_shadow[i] = calloc(len, sizeof(t_color));
		j = 0;
		while (j < len)
		{
			dst->cycle_shadow[i][j].r = src->cycle_shadow[i][j].r;
			dst->cycle_shadow[i][j].g = src->cycle_shadow[i][j].g;
			dst->cycle_shadow[i][j].b = src->cycle_shadow[i][j].b;
			dst->cycle_shadow[i][j].a = src->cycle_shadow[i][j].a;
			++j;
		}
		++i;
	}
	return;
}
