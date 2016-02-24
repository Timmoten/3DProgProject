#pragma once
#ifndef OBJREADER_H
#define OBJREADER_H

//For Reading file
#include <fstream>
//#include <string>
#include <iostream>

//For storing file data
#include <vector>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Mesh structures
//#include "Mesh.h"

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
	std::string fileName,
	std::vector<glm::vec3> *out_vertices,
	std::vector<glm::vec3> *out_normals,
	std::vector<glm::vec2> *out_uvs
	//std::vector<Texture> *out_textures
	)
{
	std::string line;
	std::ifstream myFile(fileName);
	
	if (myFile.good() == false)
	{
		std::cout << "ERROR! UNABLE TO READ FILE: '" << fileName << "'." << std::endl;
		return;
	}
	


	std::istringstream inputString;
	
	std::vector<std::string> mtlFileName;
	std::vector<std::string> mtlName;
	
	struct Group { 
		
	};
	

	std::string tempStr;
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<face> faces;

	//Vertex tempVertex;

	glm::vec3 tempVert;
	glm::vec3 tempNorm;
	glm::vec2 tempUv;
	face tempFace;
	
	while (std::getline(myFile, line))
	{
		if (line.substr(0, 7) == "mtllib ")
		{
			inputString = std::istringstream(line.substr(7));
			inputString >> tempStr;
			mtlFileName.push_back(tempStr);
		}
		if (line.substr(0, 7) == "usemtl ")
		{
			inputString = std::istringstream(line.substr(7));
			inputString >> tempStr;
			mtlName.push_back(tempStr);
		}
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
		out_vertices->push_back(vertices[--faces[i].vert1.v]);
		out_vertices->push_back(vertices[--faces[i].vert2.v]);
		out_vertices->push_back(vertices[--faces[i].vert3.v]);

		out_uvs->push_back(uvs[--faces[i].vert1.uv]);
		out_uvs->push_back(uvs[--faces[i].vert2.uv]);
		out_uvs->push_back(uvs[--faces[i].vert3.uv]);

		out_normals->push_back(normals[--faces[i].vert1.n]);
		out_normals->push_back(normals[--faces[i].vert2.n]);
		out_normals->push_back(normals[--faces[i].vert3.n]);
	}

	myFile.close();
	return;
}

#endif