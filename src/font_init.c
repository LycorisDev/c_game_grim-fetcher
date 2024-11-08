#include "grim_fetcher.h"

static t_ivec2 *get_outline_offsets(void);
static void    set_text_to_white(t_spr *s, int cycle_index);
static void    add_outline(t_spr *s, int cycle_index, t_ivec2 *offsets);

/* White text and black outline */
void add_outline_to_font(t_spr *font)
{
    int     cycle_index;
    t_ivec2 *offsets;

    offsets = get_outline_offsets();
    if (!offsets)
        return;
    cycle_index = 0;
    while (cycle_index < font->cycle_len)
    {
        set_text_to_white(font, cycle_index);
        add_outline(font, cycle_index, offsets);
        ++cycle_index;
    }
    free(offsets);
    return;
}

static t_ivec2 *get_outline_offsets(void)
{
    t_ivec2 *offsets;

    offsets = malloc(8 * sizeof(t_ivec2));
    if (!offsets)
        return 0;
    set_ivec2(offsets + 0, -1, 0);
    set_ivec2(offsets + 1, 1, 0);
    set_ivec2(offsets + 2, 0, -1);
    set_ivec2(offsets + 3, 0, 1);
    set_ivec2(offsets + 4, -1, -1);
    set_ivec2(offsets + 5, 1, -1);
    set_ivec2(offsets + 6, -1, 1);
    set_ivec2(offsets + 7, 1, 1);
    return offsets;
}

static void set_text_to_white(t_spr *s, int cycle_index)
{
    int i;
    int len;

    i = 0;
    len = s->size.x * s->size.y;
    while (i < len)
    {
        if (get_alpha(s->cycle[cycle_index][i]))
            s->cycle[cycle_index][i] = 0xFFFFFFFF;
        ++i;
    }
    return;
}

static void add_outline(t_spr *s, int cycle_index, t_ivec2 *offsets)
{
    int     i;
    int     j;
    int     len;
    t_ivec2 p;

    i = 0;
    len = s->size.x * s->size.y;
    while (i < len)
    {
        if (s->cycle[cycle_index][i] == 0xFFFFFFFF)
        {
            j = 0;
            while (j < 8)
            {
                p.y = i / s->size.x;
                p.x = i - p.y * s->size.x;
                set_ivec2(&p, p.x + offsets[j].x, p.y + offsets[j].y);
                if (s->cycle[cycle_index][p.y * s->size.x + p.x] != 0xFFFFFFFF)
                    s->cycle[cycle_index][p.y * s->size.x + p.x] = 0xFF000000;
                ++j;
            }
        }
        ++i;
    }
    return;
}
