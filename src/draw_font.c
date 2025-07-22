#include "grim_fetcher.h"

#define PAD    4
#define SIZE_X 7
#define SIZE_Y 12
#define MOD_X  4
#define MOD_Y  4

static void fix_initial_pos(t_ivec2 *pos);
static int  get_index(char c, int cycle_len);
static void draw_char(t_frame *frame, t_spr *sprite, t_ivec2 pos);
static void alignment_left(char *str, int *i, t_ivec2 *pos);

/*
   The capacity is 90 characters per line ([0-89]).
   The index is the result of `(pos->x - PAD) / SIZE_X`.
   */
void draw_font_default(t_frame *frame, t_ivec2 *pos, char *str)
{
	int   i;
	int   len;
	t_spr *s;

	if (!str)
		return;
	i = 0;
	len = strlen(str);
	s = &man.sprites[2];
	fix_initial_pos(pos);
	while (i <= len)
	{
		s->cycle_index = get_index(str[i], s->cycle_len);
		alignment_left(str, &i, pos);
		draw_char(frame, s, *pos);
		pos->x += SIZE_X;
		if (str[i] == '\n')
			set_ivec2(pos, PAD, pos->y + SIZE_Y * 2);
		if ((pos->x + SIZE_X + PAD) >= RES_WIDTH)
			set_ivec2(pos, PAD, pos->y + SIZE_Y);
		if ((pos->y + SIZE_Y + PAD) >= RES_HEIGHT)
			break;
		++i;
	}
	return;
}

static void fix_initial_pos(t_ivec2 *pos)
{
	if (pos->x < PAD)
		pos->x = PAD;
	if (pos->y < PAD)
		pos->y = PAD;
	if (pos->x % SIZE_X != MOD_X)
		pos->x -= pos->x % SIZE_X + MOD_X;
	if (pos->y % SIZE_Y != MOD_Y)
		pos->y -= pos->y % SIZE_Y + MOD_Y;
	return;
}

static int get_index(char c, int cycle_len)
{
	int index;

	index = c - ' ';
	if (index < 0 || index >= cycle_len)
		return 0;
	return index;
}

static void draw_char(t_frame *frame, t_spr *sprite, t_ivec2 pos)
{
	int     i;
	int     len;
	t_ivec2 p;

	i = 0;
	len = sprite->size.x * sprite->size.y;
	while (i < len)
	{
		p.y = i / sprite->size.x;
		p.x = i - p.y * sprite->size.x;
		set_ivec2(&p, p.x + pos.x, p.y + pos.y);
		draw_point(frame, sprite->cycle[sprite->cycle_index][i], p.x, p.y);
		++i;
	}
	return;
}

static void alignment_left(char *str, int *i, t_ivec2 *pos)
{
	int  i_curr_space;
	char *p_next_space;

	if (str[*i] != ' ')
		return;
	if (pos->x == PAD)
	{
		while (str[*i] == ' ')
			++*i;
		return;
	}
	i_curr_space = (pos->x - PAD) / SIZE_X;
	p_next_space = strchr(str + *i + 1, ' ');
	if (p_next_space && i_curr_space + p_next_space - (str + *i) > 89)
		pos->x += (89 - i_curr_space) * SIZE_X;
	return;
}
