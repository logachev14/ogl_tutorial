#include "Input.hpp"


Input::Input(GLFWwindow* window) : m_exit(false), m_window(window)
{
	m_update_thread = new std::thread(&Input::update, this);
	//m_update_thread->join();
	m_update_thread->detach();
}
void Input::update()
{
	
	GLdouble current_time = 0;
	GLdouble last_time = 0;
	GLfloat delta_time = 0;

	GLdouble xpos = 0;
	GLdouble ypos = 0;

	GLint width = 0;
	GLint height = 0;

	static const GLfloat speed = 3.0f; // 3 units / second
	static const GLdouble mouseSpeed = 0.05f;

	GLdouble horizontalAngle = 0;
	GLdouble verticalAngle = 0;

	glm::vec3 position = glm::vec3(4, 3, -5);

	GLfloat initialFoV = 110.0f;

	//static bool time_to_exit = false;
	while (!m_exit)
	{
		current_time = glfwGetTime();
		delta_time = float(current_time - last_time);
		

		glfwGetWindowSize(m_window, &width, &height);
		glfwGetCursorPos(m_window, &xpos, &ypos);

		glfwSetCursorPos(m_window, width/2, height/2);

		//вычисляем углы
		horizontalAngle += mouseSpeed * delta_time * GLfloat(width / 2 - xpos);
		verticalAngle += mouseSpeed * delta_time * GLfloat(height / 2 - ypos);


		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		glm::vec3 up = glm::cross(right, direction);

		// Движение вперед
		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
			position += direction * delta_time * speed;
		}
		// Движение назад
		if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= direction * delta_time * speed;
		}
		// Стрэйф вправо
		if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * delta_time * speed;
		}
		// Стрэйф влево
		if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * delta_time * speed;
		}

		float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

		m_lock.lock();
		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		m_projection_matrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		m_view_matrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
		
		m_lock.unlock();
		last_time = current_time;
		
		
		//std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}


glm::mat4 Input::getProjection()
{
	glm::mat4 result;
	m_lock.lock();
	result = m_projection_matrix;
	m_lock.unlock();
	return result;
}
glm::mat4 Input::getView()
{
	glm::mat4 result;
	m_lock.lock();
	result = m_view_matrix;
	m_lock.unlock();
	return result;
}

Input::~Input()
{
	m_lock.lock();
	m_exit = true;
	m_lock.unlock();
}