#pragma once
#ifndef MESH_H
#define MESH_H

//GLEW
#include <GL/glew.h>

//GLFW
//#include <GLFW\glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Vector
#include <vector>

//Shader class
#include "Shader.h"

//SOIL - Images
#include <SOIL.h>

class Mesh {
public:
	//Constructors
	Mesh(std::vector<glm::vec3> verts, std::vector<glm::vec3> norms, std::vector<glm::vec2> uvs);
	Mesh(std::vector<glm::vec3> verts, std::vector<glm::vec3> norms, std::vector<glm::vec2> uvs, std::vector<GLuint> texts);
	Mesh(const Mesh &other);
	~Mesh();

	//Renders the mesh, it will send it's textures to shader
	void Draw(Shader shader);
private:
	//Vertex information
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvCoords;

	//Contains the id of the textures
	std::vector<GLuint> textures;

	//Buffers   pos   uv     norm
	GLuint VAO, VBO0, VBO1, VBO2;

	void setupMesh();
};

Mesh::Mesh(std::vector<glm::vec3> verts, std::vector<glm::vec3> norms, std::vector<glm::vec2> uvs)
{
	vertices = verts;
	normals = norms;
	uvCoords = uvs;

	GLuint defText;
	glGenTextures(1, &defText);
	glBindTexture(GL_TEXTURE_2D, defText); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
										   // Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("default.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	std::vector<GLuint> temptexts;
	temptexts.push_back(defText);

	textures = temptexts;

	setupMesh();
}
Mesh::Mesh(std::vector<glm::vec3> verts, std::vector<glm::vec3> norms, std::vector<glm::vec2> uvs, std::vector<GLuint> texts)
{
	vertices = verts;
	normals = norms;
	uvCoords = uvs;
	textures = texts;

	setupMesh();
}
Mesh::Mesh(const Mesh &other)
{
	this->vertices = other.vertices;
	this->normals = other.normals;
	this->uvCoords = other.uvCoords;
	this->textures = other.textures; //Might bring bugs depending on how OpenGL handles the ID of textures

	setupMesh();
}
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO0);
	glDeleteBuffers(1, &VBO1);
}
void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertice Position data
	glGenBuffers(1, &VBO0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO0);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//UV data and buffer
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, uvCoords.size() * sizeof(uvCoords[0]), &uvCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(glGetUniformLocation(shader.Program, "ourTexture"), 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}




//
//#include <iostream>
//#include <vector>
//
////Shader
//#include "Shader.h"
//
////GLEW
////#include <GL/glew.h>
//
////GLFW
//#include <GLFW\glfw3.h>
//
////GLM
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//struct Vertex {
//	glm::vec3 Position;
//	glm::vec3 Normal;
//	glm::vec2 TexCoords;
//};
//struct Texture {
//	GLuint id;
//	std::string type;
//};
//
//class Mesh {
//public:
//	/*  Mesh Data  */
//	std::vector<Vertex> vertices;
//	std::vector<GLuint> indices;
//	std::vector<Texture> textures;
//	/*  Functions  */
//	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
//	void Draw(Shader shader);
//private:
//	/*  Render data  */
//	GLuint VAO, VBO, EBO;
//	/*  Functions    */
//	void setupMesh();
//};
//
//Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
//{
//	this->vertices = vertices;
//	this->indices = indices;
//	this->textures = textures;
//
//	this->setupMesh();
//}
//
//void Mesh::setupMesh()
//{
//	glGenVertexArrays(1, &this->VAO);
//	glGenBuffers(1, &this->VBO);
//	glGenBuffers(1, &this->EBO);
//
//	glBindVertexArray(this->VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->indices[0], GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
//
//	//Vertex Positions
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
//
//	//Vertex Normals
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
//
//	//Vertex Texture coordinates
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FALSE, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
//
//	glBindVertexArray(0);
//}
//
//void Mesh::Draw(Shader shader)
//{
//	GLuint diffuseNr = 1;
//	GLuint specularNr = 1;
//	
//	for (GLuint i = 0; i < this->textures.size(); i++)
//	{
//		glActiveTexture(GL_TEXTURE0 + i);
//
//		std::stringstream ss;
//		std::string number;
//		std::string name = this->textures[i].type;
//
//		if (name == "texture_diffuse")
//			ss << diffuseNr++;
//		else if (name == "texture_specular")
//			ss << specularNr++;
//		number = ss.str();
//
//		glUniform1f(glGetUniformLocation(shader.Program, ("material." + name + number).c_str()), i);
//		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
//	}
//	glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);
//
//	//Draw Mesh
//	glBindVertexArray(this->VAO);
//	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	// Always good practice to set everything back to defaults once configured.
//	for (GLuint i = 0; i < this->textures.size(); i++)
//	{
//		glActiveTexture(GL_TEXTURE0 + i);
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//}
//
#endif