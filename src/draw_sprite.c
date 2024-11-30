#include "grim_fetcher.h"

static void draw_shadow(t_frame *frame, t_spr *sprite, t_ivec2 pos);
static void draw_regular(t_frame *frame, t_spr *sprite, t_ivec2 pos);

void draw_sprite(t_frame *frame, t_spr *sprite, t_ivec2 pos, long dt_ms)
{
    if (!sprite)
        return;
    if (!dt_ms)
        sprite->cycle_index = sprite->still_frame;
    if (sprite->cycle_shadow)
        draw_shadow(frame, sprite, pos);
    draw_regular(frame, sprite, pos);
    if (!dt_ms || !sprite->cycle_time_in_ms)
        return;
    sprite->elapsed_time_in_ms += dt_ms;
    while (sprite->elapsed_time_in_ms >= sprite->cycle_time_in_ms)
    {
        sprite->cycle_index = (sprite->cycle_index + 1) % sprite->cycle_len;
        sprite->elapsed_time_in_ms -= sprite->cycle_time_in_ms;
    }
    return;
}

t_spr *get_sprite(char *id)
{
    int i;

    i = 0;
    while (man.sprites[i].id)
    {
        if (!strcmp(man.sprites[i].id, id))
            return &man.sprites[i];
        ++i;
    }
    return 0;
}

void copy_frame(t_frame *dst, t_frame *src, int zoom_factor)
{
    t_ivec2 dst_pos;
    t_ivec2 src_pos;
    t_color color;

    if (zoom_factor < 1)
        return;
    dst_pos.y = 0;
    while (dst_pos.y < dst->size.y)
    {
        dst_pos.x = 0;
        while (dst_pos.x < dst->size.x)
        {
            src_pos.x = dst_pos.x / zoom_factor;
            src_pos.y = dst_pos.y / zoom_factor;
            color = get_frame_color(src, src_pos.x, src_pos.y);
            draw_point(dst, color, dst_pos.x, dst_pos.y);
            ++dst_pos.x;
        }
        ++dst_pos.y;
    }
    return;
}

static void draw_shadow(t_frame *frame, t_spr *sprite, t_ivec2 pos)
{
    size_t  i;
    size_t  len;
    t_ivec2 p;

    pos.x += sprite->shadow_offset.x;
    pos.y += sprite->shadow_offset.y;
    i = 0;
    len = sprite->size.x * sprite->size.y;
    while (i < len)
    {
        p.y = i / sprite->size.x;
        p.x = i - p.y * sprite->size.x;
        set_ivec2(&p, p.x + pos.x, p.y + pos.y);
        draw_point(frame, sprite->cycle_shadow[sprite->cycle_index][i], p.x, p.y);
        ++i;
    }
    return;
}

static void draw_regular(t_frame *frame, t_spr *sprite, t_ivec2 pos)
{
    size_t  i;
    size_t  len;
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
