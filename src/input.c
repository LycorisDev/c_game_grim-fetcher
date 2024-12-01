#include "grim_fetcher.h"

/*
    By default, the window closing event is only triggered by the cross window 
    button or Alt+F4.
*/
void physical_key_callback(GLFWwindow *window, int key, int scancode,
    int action, int mods)
{
    (void)scancode;
    (void)mods;
    if (action != GLFW_PRESS)
        return;
    else if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if (key == GLFW_KEY_F11)
        toggle_fullscreen(window);
    else if (man.state == ONGOING)
    {
        if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
            move_player(0, -1);
        else if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
            move_player(0, 1);
        else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
            move_player(1, 0);
        else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
            move_player(-1, 0);
    }
    else if (key == GLFW_KEY_ENTER)
    {
        if (!reload_game())
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    return;
}

void scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
    (void)window;
    (void)x_offset;
    if (man.zoom <= 3)
        man.zoom = i_clamp(man.zoom + y_offset, 1, 10);
    else
        man.zoom = i_clamp(man.zoom + y_offset * 2, 1, 10);
    return;
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    (void)window;
    (void)mods;
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        set_ivec2(&man.click_pos, man.cursor.x, man.cursor.y);
        if (action == GLFW_PRESS)
        {
            list_clear(&man.clicked_path, 0);
            man.click = 1;
        }
        else if (action == GLFW_RELEASE)
            man.click = 0;
    }
    return;
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void)window;
    man.cursor.x = ((int)xpos - man.res.viewport_offset.x)
        * man.res.window_size_default.x / man.res.viewport_size.x;
    man.cursor.y = ((int)ypos - man.res.viewport_offset.y)
        * man.res.window_size_default.y / man.res.viewport_size.y;
    if (man.cursor.x < 0 || man.cursor.y < 0)
    {
        set_ivec2(&man.cursor, -1, -1);
        return ;
    }
    return;
}
