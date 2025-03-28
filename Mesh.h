#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>
#include <iostream>
#include <fstream>

#include"VAO.h"
#include"EBO.h"
#include"Texture.h"

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	// Store VAO in public so it can be used in the Draw function
	VAO VAO;

	// Initializes the mesh from obj file
	Mesh(const char* filepath);

	void Delete();
};
#endif