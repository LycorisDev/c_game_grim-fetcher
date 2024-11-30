#include "grim_fetcher.h"

static GLuint VBO;
static GLuint VAO;
static GLuint EBO;

static GLdouble vertex_data[] =
{
    -1.0, -1.0,    0.0, 1.0,
     1.0, -1.0,    1.0, 1.0,
     1.0,  1.0,    1.0, 0.0,
    -1.0,  1.0,    0.0, 0.0,
};
static GLuint indices[] = 
{
    0, 1, 2, 
    0, 2, 3
};

int create_mesh(void)
{
    int i;
    int attr_nbr;
    int attr_len;

    attr_nbr = 2;
    attr_len = 2;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, 
        GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 
        GL_STATIC_DRAW);
    i = 0;
    while (i < attr_nbr)
    {
        glEnableVertexArrayAttrib(VAO, i);
        glVertexAttribPointer(i, attr_nbr, GL_DOUBLE, GL_FALSE, 
            attr_nbr * attr_len * sizeof(GLdouble), 
            (void *)(attr_len * i * sizeof(GLdouble)));
        ++i;
    }
    return 1;
}

void render_mesh(void)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
    return;
}

void free_mesh(void)
{
    GLint i;
    GLint len;

    i = 0;
    len = 0;
    if (EBO)
        glDeleteBuffers(1, &EBO);
    if (!VAO)
        return;
    glBindVertexArray(VAO);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &len);
    i = 0;
    while (i < len)
    {
        VBO = 0;
        glGetVertexAttribIuiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &VBO);
        if (VBO)
            glDeleteBuffers(1, &VBO);
        ++i;
    }
    glDeleteVertexArrays(1, &VAO);
    return;
}
