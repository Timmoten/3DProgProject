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

struct objVertice { int v, uv, n; };
struct face { objVertice vert1, vert2, vert3; };


void removeSlash(std::string &str)
{
	for (int i = 0; i < str.length(); i++)
		if (str[i] == '/')
			str[i] = ' ';
	return;
}

void readOBJ(
	std::vector<glm::vec3> out_vertices,
	std::vector<glm::vec2> out_uvs,
	std::vector<glm::vec3> out_normals,
	std::vector<face> out_faces
	)
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
			
			inputString >> tempFace.vert1.v >> tempFace.vert1.uv >> tempFace.vert1.n
				>> tempFace.vert2.v >> tempFace.vert2.uv >> tempFace.vert2.n
				>> tempFace.vert3.v >> tempFace.vert3.uv >> tempFace.vert3.n;
			
			faces.push_back(tempFace);
			
			
		}
	}

	for (int i = 0; i < faces.size(); i++)
	{
		faces[i].vert1.v--;
		faces[i].vert2.v--;
		faces[i].vert3.v--;
		faces[i].vert1.uv--;
		faces[i].vert2.uv--;
		faces[i].vert3.uv--;
		faces[i].vert1.n--;
		faces[i].vert2.n--;
		faces[i].vert3.n--;
	}


	for (int i = 0; i < vertices.size(); i++)
	{
		out_vertices.push_back(vertices[i]);
		std::cout << "v[" << i << "].x = " << vertices[i].x << " ";
		std::cout << "v[" << i << "].y = " << vertices[i].y << " ";
		std::cout << "v[" << i << "].z = " << vertices[i].z << std::endl;
	}
	for (int i = 0; i < uvs.size(); i++)
	{
		out_uvs.push_back(uvs[i]);
		std::cout << "uv[" << i << "].x = " << uvs[i].x << " ";
		std::cout << "uv[" << i << "].y = " << uvs[i].y << std::endl;
	}
	for (int i = 0; i < normals.size(); i++)
	{
		out_normals.push_back(normals[i]);
		std::cout << "n[" << i << "].x = " << normals[i].x << " ";
		std::cout << "n[" << i << "].y = " << normals[i].y << " ";
		std::cout << "n[" << i << "].z = " << normals[i].z << std::endl;
	}
	for (int i = 0; i < faces.size(); i++)
	{
		out_faces.push_back(faces[i]);
		std::cout << "f[" << i << "].vert1.v = " << faces[i].vert1.v << " ";
		std::cout << "f[" << i << "].vert1.uv = " << faces[i].vert1.uv << " ";
		std::cout << "f[" << i << "].vert1.n = " << faces[i].vert1.n << std::endl;
		std::cout << "f[" << i << "].vert2.v = " << faces[i].vert2.v << " ";
		std::cout << "f[" << i << "].vert2.uv = " << faces[i].vert2.uv << " ";
		std::cout << "f[" << i << "].vert2.n = " << faces[i].vert2.n << std::endl;
		std::cout << "f[" << i << "].vert3.v = " << faces[i].vert3.v << " ";
		std::cout << "f[" << i << "].vert3.uv = " << faces[i].vert3.uv << " ";
		std::cout << "f[" << i << "].vert3.n = " << faces[i].vert3.n << std::endl;
	}

	myFile.close();
	return;
}

#endif