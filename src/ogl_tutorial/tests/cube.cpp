#include "cube.hpp"
#include <memory.h>

void Cube::get_buffer(GLfloat* data, GLuint* size)
{
    memcpy(data, g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
    *size = sizeof(g_vertex_buffer_data);
}

GLuint Cube::get_buff_size()
{
    return sizeof(g_vertex_buffer_data);
}