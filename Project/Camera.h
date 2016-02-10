#pragma once
#ifndef CAMERA_H
#define CAMERA_H

//GLEW
#include <GL/glew.h>

//GLFW
#include <GLFW\glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
	GLfloat pitch = 0.0f;
	GLfloat lastX;
	GLfloat lastY;
	bool firstMouse;
	bool keys[1024];

	Camera();
	Camera(GLuint W, GLuint H);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_Callback(GLFWwindow* window, double xpos, double ypos);
	void do_movement(GLfloat dT);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void key_Callback(GLFWwindow* window, int key, int scancode, int action, int mode);

};




#endif
