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

//OBJloading
#include "OBJreader.h"
#include "Mesh.h"

//loads Texture from a very specific .h file. Probably a throwaway function.
void loadTexture(GLuint *texture, GLuint width, GLuint height, unsigned char* data);

//Prints the OpenGL version the program is using, must be put after glfwMakeContextCurrent (I believe)
void printOGLVersion();

int main()
{
	// Deltatime
	GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
	GLfloat lastFrame = 0.0f;  	// Time of last frame

	const GLuint Width = 800, Height = 600;
	GLchar* VS = "vertex.glsl";
	GLchar* GS = "geometry.glsl";
	GLchar* FS = "frag.glsl";

	//initializes GLFW
	glfwInit();
	
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//version 3.3
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

	//Prints OpenGL version in use
	printOGLVersion();


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



	//GLfloat vertices[] =
	//{
	//	// Positions          // Texture Coords
	//	0.5f,  0.5f, 0.0f,   1.0f, 1.0f,		// Top Right
	//	0.5f, -0.5f, 0.0f,   1.0f, 0.0f,		// Bottom Right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,		// Bottom Left
	//	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f		// Top Left 
	//};

	GLfloat vertices[] =
	{
		// Positions          // Texture Coords
		0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Shader ourShader(VS, GS, FS);

	//Texture
	GLuint texture;
	loadTexture(&texture, BTH_IMAGE_WIDTH, BTH_IMAGE_HEIGHT, BTH_IMAGE_DATA);

	//Test readOBJ
	std::string fileName = "sphere.obj";
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec3> out_normals;
	std::vector<glm::vec2> out_uvs;
	std::vector<face> out_indices;
	//std::vector<Texture> out_textures;
	readOBJ(
		fileName,
		&out_vertices,
		&out_normals,
		&out_uvs
		//&out_textures
		);


	//Currently a sphere
	GLuint VAO2, VBO2, VBO3;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	//Vertice data and buffer
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//UV data and buffer
	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(glm::vec2), &out_uvs[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);


	//Square
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
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
		model = glm::rotate(model, currentFrame*glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // alternative, in the assignment glm::degrees(-0.1f) was the speed requested.
		glm::mat4 view; //View matrix
		view = cam.getView();
		glm::mat4 projection; //Projection matrix
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)Width / (GLfloat)Height, 0.5f, 30.0f);

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		/*glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);*/

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, out_vertices.size());
		glBindVertexArray(0);
		 
		glfwSwapBuffers(window); //swap the buffers
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(ourShader.Program);

	glfwTerminate();
	return 0;
}

void loadTexture(GLuint *texture, GLuint width, GLuint height, unsigned char* data)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture); // all following GL_TEXTURE_2D operations affect this texture now

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load, create, texture and generate mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //check bth_image.h
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return;
}

void printOGLVersion()
{
	/*
	Shows the OpenGl version currently in use. If glfWindowHint is commented out, it should by default
	try to use the the latest supported version, so you could use this to test if you're sitting on a computer
	that doesn't support the version you want to use...
	*/
	const GLubyte *test = glGetString(GL_VERSION);
	if (test != nullptr)
	{
		std::cout << "OpenGL version used: ";
		int testInt = 0;
		while (test[testInt] != '\0')
		{
			std::cout << test[testInt];
			testInt++;
		}
	}
	std::cout << std::endl;
}