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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); //creates the callback?

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

int main()
{
	GLchar* VS = "vertex.glsl";
	GLchar* GS = "geometry.glsl";
	GLchar* FS = "frag.glsl";
	GLint Width = 800;
	GLint Height = 600;
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

	glfwSetKeyCallback(window, key_callback); // registering the callback function

	glewExperimental = GL_TRUE; //avoids errors with newer versions of opengl
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600); // starts in the lower left corner with 0,0 to 800,600  w,h
	glEnable(GL_DEPTH_TEST);

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
		glfwPollEvents(); //awaits button press
		
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//uses shader program
		ourShader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);
		 
		glm::mat4 model; //Model matrix
		model = glm::rotate(model, (GLfloat)glfwGetTime()*glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // alternative, in the assignment glm::degrees(-0.1f) was the speed requested.
		glm::mat4 view; //View matrix
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection; //Projection matrix
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)640.0f / (GLfloat)480.0f, 0.5f, 20.0f);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // if escape is presseed
		glfwSetWindowShouldClose(window, GL_TRUE); // close window


}