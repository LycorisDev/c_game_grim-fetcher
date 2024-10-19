#include "grim_fetcher.h"

static t_ubyte	get_ubyte(char c);

t_uint	get_color(t_ubyte alpha, t_ubyte red, t_ubyte green, t_ubyte blue)
{
	return (alpha << 24 | red << 16 | green << 8 | blue);
}

/* e.g.: "#097e7b" - "097e7b" */
t_uint	get_color_from_hex(char *str)
{
	int		i;
	t_uint	color;
	t_ubyte	values[3];

	i = str[0] == '#';
	color = 0;
	values[0] = get_ubyte(str[i + 0]) * 16 + get_ubyte(str[i + 1]);
	values[1] = get_ubyte(str[i + 2]) * 16 + get_ubyte(str[i + 3]);
	values[2] = get_ubyte(str[i + 4]) * 16 + get_ubyte(str[i + 5]);
	set_alpha(&color, 255);
	set_red(&color, values[0]);
	set_green(&color, values[1]);
	set_blue(&color, values[2]);
	return (color);
}

t_uint	get_alpha_blended_color(t_ubyte prev[4], t_ubyte curr[4])
{
	t_ubyte	blended[4];
	float	alpha_prev;
	float	alpha_curr;
	float	alpha_blend;
	float	tmp;

	if (curr[0] == 255)
		return (get_color(curr[0], curr[1], curr[2], curr[3]));
	alpha_prev = prev[3] / 255.0f;
	alpha_curr = curr[3] / 255.0f;
	alpha_blend = alpha_prev + alpha_curr * (1.0f - alpha_prev);
	if (alpha_blend == 0)
		return (0);
	blended[0] = (t_uint)(alpha_blend * 255.0f) % 256;
	tmp = prev[1] * alpha_prev + curr[1] * alpha_curr * (1.0f - alpha_prev);
	blended[1] = (t_uint)(tmp / alpha_blend) % 256;
	tmp = prev[2] * alpha_prev + curr[2] * alpha_curr * (1.0f - alpha_prev);
	blended[2] = (t_uint)(tmp / alpha_blend) % 256;
	tmp = prev[3] * alpha_prev + curr[3] * alpha_curr * (1.0f - alpha_prev);
	blended[3] = (t_uint)(tmp / alpha_blend) % 256;
	return (get_color(blended[0], blended[1], blended[2], blended[3]));
}

static t_ubyte	get_ubyte(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'a' && c <= 'f')
		return (10 + (c - 'a'));
	else if (c >= 'A' && c <= 'F')
		return (10 + (c - 'A'));
	return (0);
}
