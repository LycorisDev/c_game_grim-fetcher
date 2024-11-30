#include "grim_fetcher.h"

static void set_res(const GLFWvidmode *vid_mode);
static void set_viewport(int framebuffer_size_x, int framebuffer_size_y);
static void set_initial_viewport(GLFWwindow *window);
static void framebuffer_size_callback(GLFWwindow *window, int x, int y);
static void window_pos_callback(GLFWwindow *window, int xpos, int ypos);

GLFWwindow *get_window(char *title)
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        fprintf(stderr, "Error: The GLFW library failed to initialize\n");
        exit(EXIT_FAILURE);
    }
    set_res(glfwGetVideoMode(glfwGetPrimaryMonitor()));
    #ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    window = glfwCreateWindow(man.res.window_size.x, man.res.window_size.y,
        title, NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Error: The window failed to create (the issue may be "
            "with the GLFW library, but comes more likely from the OpenGL "
            "library / check that your drivers are properly installed)\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowSizeLimits(window, 
        man.res.window_size.x, man.res.window_size.y, 
        man.res.monitor_size.x, man.res.monitor_size.y);
    glfwMakeContextCurrent(window);
    set_initial_viewport(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowPosCallback(window, window_pos_callback);
    if (!init_gl_functions())
    {
        fprintf(stderr, "Error: Failed to load OpenGL functions\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    return window;
}

void toggle_fullscreen(GLFWwindow *window)
{
    int               decorated;
    GLFWmonitor       *monitor;
    const GLFWvidmode *mode;

    decorated = !glfwGetWindowAttrib(window, GLFW_DECORATED);
    if (decorated)
    {
        glfwSetWindowMonitor(window, NULL, 
            man.res.window_position.x, man.res.window_position.y, 
            man.res.window_size.x, man.res.window_size.y, GLFW_DONT_CARE);
    }
    else
    {
        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, man.res.fullscreen.x, 
            man.res.fullscreen.y, man.res.monitor_size.x, 
            man.res.monitor_size.y, mode->refreshRate);
    }
    glfwSetWindowAttrib(window, GLFW_DECORATED, decorated);
    return;
}

static void set_res(const GLFWvidmode *vid_mode)
{
    man.res.monitor_size.x = vid_mode->width;
    man.res.monitor_size.y = vid_mode->height;
    man.res.aspect_ratio = (double)man.res.monitor_size.x / 
        man.res.monitor_size.y;
    if (man.res.aspect_ratio > 16.0 / 9)
    {
        man.res.aspect_ratio = 16.0 / 9;
        man.res.monitor_size.x = man.res.monitor_size.y * man.res.aspect_ratio;
    }
    man.res.window_size_default.x = i_min(640, man.res.monitor_size.x);
    man.res.window_size_default.y = i_min(640 / man.res.aspect_ratio, 
        man.res.monitor_size.y);
    man.res.window_size.x = man.res.window_size_default.x;
    man.res.window_size.y = man.res.window_size_default.y;
    man.res.window_position.x = 0;
    man.res.window_position.y = 0;
    man.res.fullscreen.x = (vid_mode->width - man.res.monitor_size.x) / 2;
    man.res.fullscreen.y = (vid_mode->height - man.res.monitor_size.y) / 2;
    return;
}

static void set_viewport(int framebuffer_size_x, int framebuffer_size_y)
{
    man.res.viewport_size.x = framebuffer_size_x;
    man.res.viewport_size.y = framebuffer_size_x / man.res.aspect_ratio;
    if (man.res.viewport_size.y > framebuffer_size_y)
    {
        man.res.viewport_size.y = framebuffer_size_y;
        man.res.viewport_size.x = framebuffer_size_y * man.res.aspect_ratio;
    }
    man.res.viewport_offset.x = (framebuffer_size_x - man.res.viewport_size.x)
        / 2;
    man.res.viewport_offset.y = (framebuffer_size_y - man.res.viewport_size.y)
        / 2;
    glViewport(man.res.viewport_offset.x, man.res.viewport_offset.y,
        man.res.viewport_size.x, man.res.viewport_size.y);
    return;
}

static void set_initial_viewport(GLFWwindow *window)
{
    t_ivec2 size;

    glfwGetFramebufferSize(window, &size.x, &size.y);
    set_viewport(size.x, size.y);
    return;
}

static void framebuffer_size_callback(GLFWwindow *window, int x, int y)
{
    (void)window;
    set_viewport(x, y);
    if (glfwGetWindowAttrib(window, GLFW_DECORATED))
    {
        man.res.window_size.x = x;
        man.res.window_size.y = y;
    }
    return;
}

static void window_pos_callback(GLFWwindow *window, int xpos, int ypos)
{
    if (glfwGetWindowAttrib(window, GLFW_DECORATED))
    {
        man.res.window_position.x = xpos;
        man.res.window_position.y = ypos;
    }
    return;
}
