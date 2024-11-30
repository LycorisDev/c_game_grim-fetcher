#include "grim_fetcher.h"

static void render_steps(t_frame *frame);
static void render_souls(t_frame *frame, long dt);

void render_gui(t_frame *frame, long dt)
{
    render_steps(frame);
    render_souls(frame, dt);
    if (man.cursor.x >= 0 && man.cursor.y >= 0)
        draw_cursor(frame, &man.sprites[1], man.cursor, man.click);
    return;
}

/*
static void render_health(t_frame *frame)
{
    t_ivec2 pos;
    char    *text;

    set_ivec2(&pos, 8, 0);
    draw_font_default(frame, &pos, " Health: ");
    text = itoa_dec(man.player.health);
    draw_font_default(frame, &pos, text);
    free(text);
    draw_font_default(frame, &pos, "%");
    return;
}

static void render_stamina(t_frame *frame)
{
    t_ivec2 pos;
    char    *text;

    set_ivec2(&pos, 0, 24);
    draw_font_default(frame, &pos, "Stamina: ");
    text = itoa_dec(man.player.stamina);
    set_ivec2(&pos, 70, 24);
    draw_font_default(frame, &pos, text);
    free(text);
    pos.y = 24;
    draw_font_default(frame, &pos, "%");
    return;
}
*/

static void render_steps(t_frame *frame)
{
    t_ivec2 pos;
    char    *text;

    set_ivec2(&pos, 300, 0);
    draw_font_default(frame, &pos, "Steps: ");
    text = itoa_dec(man.player.steps);
    draw_font_default(frame, &pos, text);
    free(text);
    return;
}

static void render_souls(t_frame *frame, long dt)
{
    t_ivec2 pos;
    char    *text;

    set_ivec2(&pos, 560, -4);
    draw_sprite(frame, &man.sprites[12], pos, dt);
    set_ivec2(&pos, 594, 0);
    text = itoa_dec(man.player.collected);
    draw_font_default(frame, &pos, text);
    free(text);
    draw_font_default(frame, &pos, "/");
    text = itoa_dec(man.player.to_collect);
    draw_font_default(frame, &pos, text);
    free(text);
    return;
}
