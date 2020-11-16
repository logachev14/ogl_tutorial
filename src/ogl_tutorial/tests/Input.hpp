#pragma once
#include <chrono>
#include <thread>
#include <mutex>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Input
{
public:
	Input(GLFWwindow* window);
	~Input();

	glm::mat4 getProjection();
	glm::mat4 getView();

private:
	std::mutex m_lock;
	void update();
	std::thread * m_update_thread;
	bool m_exit;

	GLFWwindow* m_window;

	glm::mat4 m_projection_matrix;
	glm::mat4 m_view_matrix;
};

