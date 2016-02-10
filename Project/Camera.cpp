#include "Camera.h"


Camera::Camera()
{
	GLfloat lastX = 800.0f / 2.0;
	GLfloat lastY = 600.0f / 2.0;
	firstMouse = true;
}

Camera::Camera(GLuint W, GLuint H)
{
	GLfloat lastX = W / 2.0;
	GLfloat lastY = H / 2.0;
	firstMouse = true;
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	//cameraFront = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}

void Camera::mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
	cam->mouse_callback(window, xpos, ypos);
}

void Camera::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // if escape is presseed
		glfwSetWindowShouldClose(window, GL_TRUE); // close window
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void Camera::key_Callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Camera* cam=(Camera*)glfwGetWindowUserPointer(window);
	cam->key_callback(window, key, scancode, action, mode);
	
}

void Camera::do_movement(GLfloat dT)
{
	//camera controls
	GLfloat cameraSpeed = 5.0f * dT;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_SPACE])
		cameraPos += cameraSpeed * cameraUp;
	if (keys[GLFW_KEY_C])
		cameraPos -= cameraSpeed * cameraUp;
}
