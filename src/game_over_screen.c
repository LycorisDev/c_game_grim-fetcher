#include "grim_fetcher.h"

static void render_victory_emote(t_frame *frame);
static void render_victory_msg(t_frame *frame);
static void render_reload_msg(t_frame *frame);

int game_over_screen(void)
{
    long    dt;
    t_frame *frame;

    if (man.state != VICTORY && man.state != FAILURE)
        return 0;
    dt = get_delta_time();
    frame = man.frame[man.curr_frame];
    render_background(frame);
    render_victory_emote(frame);
    render_victory_msg(frame);
    render_reload_msg(frame);
    render_gui(frame, dt);
    return 1;
}

static void render_victory_emote(t_frame *frame)
{
    t_ivec2 pos;

    set_ivec2(&pos, RES_WIDTH / 2, RES_HEIGHT / 2 - 24);
    if (man.state == VICTORY)
    {
        pos.x -= strlen("\\(^_^)/") / 2 * 8;
        draw_font_default(frame, &pos, "\\(^_^)/");
    }
    else
    {
        pos.x -= strlen("(x_x)") / 2 * 8;
        draw_font_default(frame, &pos, "(x_x)");
    }
    return;
}

static void render_victory_msg(t_frame *frame)
{
    t_ivec2 pos;

    set_ivec2(&pos, RES_WIDTH / 2, RES_HEIGHT / 2);
    if (man.state == VICTORY)
    {
        pos.x -= strlen("You won!") / 2 * 8;
        draw_font_default(frame, &pos, "You won!");
    }
    else
    {
        pos.x -= strlen("You got killed by a slime.") / 2 * 8;
        draw_font_default(frame, &pos, "You got killed by a slime.");
    }
    return;
}

static void render_reload_msg(t_frame *frame)
{
    t_ivec2 pos;

    set_ivec2(&pos, RES_WIDTH / 2, RES_HEIGHT / 2 + 24);
    if (man.state == VICTORY)
    {
        if (!man.map_filenames[man.map_index + 1])
        {
            pos.x -= strlen("Game complete!") / 2 * 8;
            draw_font_default(frame, &pos, "Game complete!");
        }
        else
        {
            pos.x -= strlen("Press Enter to continue...") / 2 * 8;
            draw_font_default(frame, &pos, "Press Enter to continue...");
        }
    }
    else
    {
        pos.x -= strlen("Press Enter to retry...") / 2 * 8;
        draw_font_default(frame, &pos, "Press Enter to retry...");
    }
    return;
}
