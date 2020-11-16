#pragma once
#include <GLFW/glfw3.h>

class IGeometry
{
public:
	virtual void get_buffer(GLfloat* data, GLuint* size) = 0;
	virtual GLuint get_buff_size() = 0;
};
