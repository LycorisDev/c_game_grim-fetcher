#include "grim_fetcher.h"

static int  init(char *title, char *map_path);
static int  game_init(char *map_path);
static void update_time_variables(void);
static void deinit(void);
static void game_deinit(void);

t_man man;

int main(int argc, char **argv)
{
    (void)argc;
    if (!init("Grim Fetcher", argv[1]))
    {
        dprintf(2, "Error: Failure during initialization\n");
        deinit();
        return EXIT_FAILURE;
    }
    while (!glfwWindowShouldClose(man.window))
    {
        update_time_variables();
        glClear(GL_COLOR_BUFFER_BIT);
        clear_drawing(man.frame[man.curr_frame], 255);
        render();
        save_drawing(man.frame[man.curr_frame]);
        render_mesh();
        man.curr_frame = (man.curr_frame + 1) % 2;
        glfwSwapBuffers(man.window);
        glfwPollEvents();
    }
    deinit();
    return EXIT_SUCCESS;
}

int reload_game(void)
{
    char *map_path;
    int  success;

    map_path = strdup(man.map.name);
    game_deinit();
    success = game_init(map_path);
    free(map_path);
    return success;
}

static int init(char *title, char *map_path)
{
    bzero(&man, sizeof(t_man));
    if (!game_init(map_path))
        return 0;
    if (!set_sprite_array("textures/index.json"))
        return 0;
    man.window = get_window(title);
    man.shader_program = create_shader_program();
    if (!man.window || !man.shader_program || !create_uniform()
        || !create_mesh() || !create_frames())
        return 0;
    add_outline_to_font(&man.sprites[2]);
    use_frame(man.frame[man.curr_frame]);
    glfwSetKeyCallback(man.window, physical_key_callback);
    glfwSetScrollCallback(man.window, scroll_callback);
    glfwSetMouseButtonCallback(man.window, mouse_callback);
    glfwSetInputMode(man.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPosCallback(man.window, cursor_pos_callback);
    return 1;
}

static int game_init(char *map_path)
{
    bzero(&man.player, sizeof(t_player));
    bzero(&man.map, sizeof(t_map));
    man.clicked_path = 0;
    set_ivec2(&man.cursor, -1, -1);
    set_ivec2(&man.click_pos, -1, -1);
    man.zoom = 5;
    man.state = ONGOING;
    if (!set_map_and_player(map_path))
        return 0;
    return 1;
}

static void update_time_variables(void)
{
    static double last_time;
    double current_time;

    current_time = glfwGetTime();
    man.delta_time = current_time - last_time;
    last_time = current_time;
    man.fps_count = 1 / man.delta_time;
    return;
}

static void deinit(void)
{
    glfwTerminate();
    free_shader_program();
    free_uniform();
    free_mesh();
    free_frames();
    free_sprites();
    game_deinit();
    return;
}

static void game_deinit(void)
{
    free(man.map.name);
    free(man.map.cells);
    list_clear(&man.clicked_path, 0);
    return;
}
