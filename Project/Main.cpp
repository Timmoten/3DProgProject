//GLEW
#include <GL/glew.h>

//GLFW
#include <GLFW\glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//SOIL
#include <SOIL.h>

//Misc
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "Shader.h"
#include "bth_image.h"
#include "Camera.h"

//function prototypes
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); //creates the callback?
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void do_movement();

//Window size
//const GLuint Width = 800, Height = 600;

//Camera
/*glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = Width / 2.0;
GLfloat lastY = Height / 2.0;*/
//bool keys[1024];

// Deltatime
//GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
//GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Deltatime
	GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
	GLfloat lastFrame = 0.0f;  	// Time of last frame

	const GLuint Width = 800, Height = 600;
	GLchar* VS = "vertex.glsl";
	GLchar* GS = "geometry.glsl";
	GLchar* FS = "frag.glsl";
	//GLint Width = 800;
	//GLint Height = 600;
	//initializes GLFW
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // using core profile
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //not resizable

	//creates the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "hl3.exe", nullptr, nullptr); // w , h , name, null, null
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//sets callback functions
	Camera cam(Width, Height);
	glfwSetWindowUserPointer(window, &cam);
	glfwSetKeyCallback(window, Camera::key_Callback); // registering the callback function
	glfwSetCursorPosCallback(window, Camera::mouse_Callback);

	//GLFW options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE; //avoids errors with newer versions of opengl
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600); // starts in the lower left corner with 0,0 to 800,600  w,h

	glEnable(GL_DEPTH_TEST); // enables depth test

	GLfloat vertices[] =
	{
		// Positions          // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f,		// Top Right
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f,		// Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,		// Bottom Left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f		// Top Left 
	};


	GLuint indices[] = {  // start at 0
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};


	Shader ourShader(VS, GS, FS);


	//Texture
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all following GL_TEXTURE_2D operations affect this texture now
										  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load, create, texture and generate mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, BTH_IMAGE_WIDTH, BTH_IMAGE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, BTH_IMAGE_DATA); //check bth_image.h
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);


	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); // gives the buffer a ID
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); //When refercing this position attribute in the shaders it will be stored at location 0
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);



	glBindVertexArray(0);// unbind VAO



	while (!glfwWindowShouldClose(window)) // a "game loop"
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents(); //awaits button press
		cam.do_movement(deltaTime);
		
		glClearColor(0.2f, 0.3f, 0.5f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//uses shader program
		ourShader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);
		 
		glm::mat4 model; //Model matrix
		//model = glm::rotate(model, (GLfloat)glfwGetTime()*glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // alternative, in the assignment glm::degrees(-0.1f) was the speed requested.
		model = glm::rotate(model, currentFrame*glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // alternative, in the assignment glm::degrees(-0.1f) was the speed requested.
		glm::mat4 view; //View matrix
		view = cam.getView();
		glm::mat4 projection; //Projection matrix
		//projection = glm::perspective(glm::radians(45.0f), (GLfloat)640.0f / (GLfloat)480.0f, 0.5f, 20.0f);
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)Width / (GLfloat)Height, 0.5f, 30.0f);

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		GLint lightColorLocation = glGetUniformLocation(ourShader.Program, "lightColor");
		glUniform4f(lightColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
		GLint lightPosLocation = glGetUniformLocation(ourShader.Program, "lightPosition");
		glUniform4f(lightPosLocation, 0.0f, 0.0f, 3.0f, 0.0f);


		glBindVertexArray(VAO);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //used with element buffer

		glBindVertexArray(0);
		 
		glfwSwapBuffers(window); //swap the buffers
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(ourShader.Program);

	glfwTerminate();
	return 0;
}

/*void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
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
}*/

/*void do_movement()
{
	//camera controls
	GLfloat cameraSpeed = 5.0f * deltaTime;
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
}*/

//bool firstMouse = true;
/*void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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
}*/