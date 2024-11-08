#include "grim_fetcher.h"

static t_ubyte get_ubyte(char c);

t_uint get_color(t_ubyte alpha, t_ubyte red, t_ubyte green, t_ubyte blue)
{
    return alpha << 24 | red << 16 | green << 8 | blue;
}

/* e.g.: "#097e7b" - "097e7b" */
t_uint get_color_from_hex(char *str)
{
    int     i;
    t_uint  color;
    t_ubyte values[3];

    i = str[0] == '#';
    color = 0;
    values[0] = get_ubyte(str[i + 0]) * 16 + get_ubyte(str[i + 1]);
    values[1] = get_ubyte(str[i + 2]) * 16 + get_ubyte(str[i + 3]);
    values[2] = get_ubyte(str[i + 4]) * 16 + get_ubyte(str[i + 5]);
    set_alpha(&color, 255);
    set_red(&color, values[0]);
    set_green(&color, values[1]);
    set_blue(&color, values[2]);
    return color;
}

t_uint get_alpha_blended_color(t_ubyte prev[4], t_ubyte curr[4])
{
    t_ubyte blend[4];

    blend[0] = curr[0] + (255 - curr[0]) * prev[0] / 255;
    if (!blend[0])
        return 0;
    blend[1] = (curr[0] * curr[1] + (255 - curr[0]) * prev[0] * prev[1] / 255)
        / blend[0];
    blend[2] = (curr[0] * curr[2] + (255 - curr[0]) * prev[0] * prev[2] / 255)
        / blend[0];
    blend[3] = (curr[0] * curr[3] + (255 - curr[0]) * prev[0] * prev[3] / 255)
        / blend[0];
    return get_color(blend[0], blend[1], blend[2], blend[3]);
}

t_ubyte get_alpha(t_uint color)
{
    return (color >> 24) & 0xFF;
}

void set_alpha(t_uint *color, t_ubyte value)
{
    *color = (*color & 0x00FFFFFF) | (value << 24);
    return;
}

t_ubyte get_red(t_uint color)
{
    return (color >> 16) & 0xFF;
}

void set_red(t_uint *color, t_ubyte value)
{
    *color = (*color & 0xFF00FFFF) | (value << 16);
    return;
}

t_ubyte get_green(t_uint color)
{
    return (color >> 8) & 0xFF;
}

void set_green(t_uint *color, t_ubyte value)
{
    *color = (*color & 0xFFFF00FF) | (value << 8);
    return;
}

t_ubyte get_blue(t_uint color)
{
    return color & 0xFF;
}

void set_blue(t_uint *color, t_ubyte value)
{
    *color = (*color & 0xFFFFFF00) | value;
    return;
}

static t_ubyte get_ubyte(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'a' && c <= 'f')
        return 10 + (c - 'a');
    else if (c >= 'A' && c <= 'F')
        return 10 + (c - 'A');
    return 0;
}
