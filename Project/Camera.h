#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

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
	glm::mat4 getView() {	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);	}

	Camera();
	Camera(GLuint W, GLuint H);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_Callback(GLFWwindow* window, double xpos, double ypos);
	void do_movement(GLfloat dT);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void key_Callback(GLFWwindow* window, int key, int scancode, int action, int mode);

private:
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;
	GLfloat yaw;
	GLfloat pitch;
	GLfloat lastX;
	GLfloat lastY;
	bool firstMouse;
	bool keys[1024];
};




#endif
