#include "grim_fetcher.h"

static int app_glsl_version = 0;

static GLuint compile_shader(GLenum type, char *filepath);
static int    get_app_glsl_version(void);
static void   set_glsl_version_in_shader(char *ptr_shader);
static void   free_shader(GLuint *id);

int create_shader_program(void)
{
    char   *vs_filepath;
    char   *fs_filepath;
    GLuint vs;
    GLuint fs;
    int    shader_program;

    vs_filepath = "shaders/vs.glsl";
    fs_filepath = "shaders/fs.glsl";
    vs = compile_shader(GL_VERTEX_SHADER, vs_filepath);
    fs = compile_shader(GL_FRAGMENT_SHADER, fs_filepath);
    if (!vs || !fs)
    {
        free_shader(&vs);
        free_shader(&fs);
        return 0;
    }
    shader_program = glCreateProgram();
    if (!shader_program)
    {
        fprintf(stderr, "Error: Couldn't compile shader program\n");
        free_shader(&vs);
        free_shader(&fs);
        return 0;
    }
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);
    free_shader(&vs);
    free_shader(&fs);
    return shader_program;
}

void free_shader_program(void)
{
    if (man.shader_program)
    {
        glUseProgram(0);
        glDeleteProgram(man.shader_program);
    }
    man.shader_program = 0;
    return;
}

static GLuint compile_shader(GLenum type, char *filepath)
{
    GLuint id_shader;
    char   *ptr;

    ptr = read_file(filepath);
    if (!ptr)
        return 0;
    else if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER)
    {
        free(ptr);
        return 0;
    }
    if (!app_glsl_version)
        app_glsl_version = get_app_glsl_version();
    set_glsl_version_in_shader(ptr);
    id_shader = glCreateShader(type);
    glShaderSource(id_shader, 1, (const GLchar **)&ptr, 0);
    glCompileShader(id_shader);
    free(ptr);
    return id_shader;
}

static int get_app_glsl_version(void)
{
    const unsigned char* gl = glGetString(GL_VERSION);
    if (gl[0]-48 >= 4)
        return 400;
    else if (gl[0]-48 == 3 && gl[2]-48 == 3)
        return 330;
    return 150;
}

static void set_glsl_version_in_shader(char *ptr_shader)
{
    int i;
    int glsl;

    i = 0;
    glsl = app_glsl_version;
    while (ptr_shader[i])
    {
        if (isdigit(ptr_shader[i]))
        {
            ptr_shader[i + 2] = glsl % 10 + '0';
            glsl /= 10;
            ptr_shader[i + 1] = glsl % 10 + '0';
            glsl /= 10;
            ptr_shader[i] = glsl % 10 + '0';
            glsl /= 10;
            break;
        }
        else if (ptr_shader[i] == '\n')
            break;
        ++i;
    }
    return;
}

static void free_shader(GLuint *id)
{
    glDeleteShader(*id);
    *id = 0;
    return;
}
