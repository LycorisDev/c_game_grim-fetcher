#include "grim_fetcher.h"

void put_pixel(t_frame *frame, t_uint color, int x, int y)
{
    char    *dst;
    t_ubyte current[4];
    t_ubyte new[4];

    dst = frame->addr + (y * frame->line_length + x * (frame->bpp / 8));
    new[0] = get_alpha(color);
    new[1] = get_red(color);
    new[2] = get_green(color);
    new[3] = get_blue(color);
    if (frame->endian)
        *(t_uint *)dst = get_alpha_blended_color((t_ubyte *)dst, new);
    else
    {
        current[0] = dst[3];
        current[1] = dst[2];
        current[2] = dst[1];
        current[3] = dst[0];
        *(t_uint *)dst = get_alpha_blended_color(current, new);
    }
    return;
}

void draw_point(t_frame *frame, t_uint color, t_ivec2 p)
{
    int i;
    int j;

    if (!get_alpha(color))
        return;
    if (p.x < 0 || p.x >= RES_WIDTH || p.y < 0 || p.y >= RES_HEIGHT)
        return;
    set_ivec2(&p, p.x * frame->thickness, p.y * frame->thickness);
    i = 0;
    while (i < frame->thickness)
    {
        j = 0;
        while (j < frame->thickness)
        {
            put_pixel(frame, color, p.x + j, p.y + i);
            ++j;
        }
        ++i;
    }
    return;
}

void draw_line(t_frame *frame, t_uint color, t_ivec2 p, t_ivec2 dir)
{
    int   steps;
    float coords_x;
    float coords_y;
    float increment_x;
    float increment_y;

    steps = i_max(i_abs(dir.x), i_abs(dir.y));
    coords_x = p.x;
    coords_y = p.y;
    increment_x = dir.x / (float)steps;
    increment_y = dir.y / (float)steps;
    ++steps;
    while (steps-- > 0)
    {
        set_ivec2(&dir, coords_x, coords_y);
        draw_point(frame, color, dir);
        coords_x += increment_x;
        coords_y += increment_y;
    }
    return;
}
