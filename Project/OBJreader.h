#pragma once
#ifndef OBJREADER_H
#define OBJREADER_H

//For Reading file
#include <fstream>
//#include <string>
#include <iostream>

//For storing file data
#include <vector>

//Replacing chars in strig
//#include <algorithm>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct face { int v, uv, n; };

void removeSlash(std::string &str)
{
	for (int i = 0; i < str.length(); i++)
		if (str[i] == '/')
			str[i] = ' ';
	return;
}

void readOBJ()
{
	std::string fileName("cube.obj"), line;
	//std::string line;
	std::ifstream myFile(fileName); //File open for input operations
	
	if (myFile.is_open())
		std::cout << "File: '" << fileName << "' is currently open." << std::endl;

	if (myFile.good() == false)
	{
		std::cout << "ERROR! UNABLE TO READ FILE: '" << fileName << "'." << std::endl;
		return;
	}
	
	std::istringstream inputString;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<face> faces;

	glm::vec3 tempVert;
	glm::vec3 tempNorm;
	glm::vec2 tempUv;
	face tempFace;
	
	while (std::getline(myFile, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			inputString = std::istringstream(line.substr(2));
			inputString >> tempVert.x >> tempVert.y >> tempVert.z;
			vertices.push_back(tempVert);
		}
		if (line.substr(0, 2) == "vn")
		{
			inputString = std::istringstream(line.substr(2));
			inputString >> tempNorm.x >> tempNorm.y >> tempNorm.z;
			normals.push_back(tempNorm);
		}
		if (line.substr(0, 2) == "vt")
		{
			inputString = std::istringstream(line.substr(2));
			inputString >> tempUv.x >> tempUv.y;
			uvs.push_back(tempUv);
		}
		if (line.substr(0, 1) == "f")
		{
			removeSlash(line);
			inputString = std::istringstream(line.substr(2));
			
			for (int i = 0; i < 3; i++)
			{
				inputString >> tempFace.v >> tempFace.uv >> tempFace.n;
				faces.push_back(tempFace);
			}
			
			
		}
	}

	for (int i = 0; i < faces.size(); i++)
	{
		faces[i].v--;
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		std::cout << "v[" << i << "].x = " << vertices[i].x << " ";
		std::cout << "v[" << i << "].y = " << vertices[i].y << " ";
		std::cout << "v[" << i << "].z = " << vertices[i].z << std::endl;
	}
	for (int i = 0; i < uvs.size(); i++)
	{
		std::cout << "uv[" << i << "].x = " << uvs[i].x << " ";
		std::cout << "uv[" << i << "].y = " << uvs[i].y << std::endl;
	}
	for (int i = 0; i < normals.size(); i++)
	{
		std::cout << "n[" << i << "].x = " << normals[i].x << " ";
		std::cout << "n[" << i << "].y = " << normals[i].y << " ";
		std::cout << "n[" << i << "].z = " << normals[i].z << std::endl;
	}
	for (int i = 0; i < faces.size(); i++)
	{
		std::cout << "f[" << i << "].v = " << faces[i].v << " ";
		std::cout << "f[" << i << "].uv = " << faces[i].uv << " ";
		std::cout << "f[" << i << "].n = " << faces[i].n << std::endl;
	}

	myFile.close();
	return;
}

#endif