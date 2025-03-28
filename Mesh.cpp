#include "Mesh.h"

Mesh::Mesh(const char* filePath)
{
	std::cout << "read mesh " << filePath << std::endl;
	
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::string myText;
	std::ifstream MyReadFile(filePath);

	std::string objFormat = "full";

	while (getline(MyReadFile, myText)) {

		if (myText.rfind("v ", 0) == 0) {
			glm::vec3 vertex;
			sscanf_s(myText.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (myText.rfind("vt ", 0) == 0) {
			glm::vec2 uv;
			sscanf_s(myText.c_str(), "vt %f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (myText.rfind("vn ", 0) == 0) {
			glm::vec3 normal;
			sscanf_s(myText.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (myText.rfind("f ", 0) == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			if (myText.find("//") != -1) {
				int matches = sscanf_s(myText.c_str(), "f %d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				objFormat = "noUv";
			}
			else if (myText.find("/") != -1) {
				int matches = sscanf_s(myText.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				objFormat = "full";
			}
			else {
				int matches = sscanf_s(myText.c_str(), "f %d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				objFormat = "noUvNoNormals";
			}
		}
	}

	for (int i = 0; i < vertexIndices.size(); i++) {

		Vertex vertex = Vertex();
		if (objFormat == "full") {
			vertex.position = temp_vertices[vertexIndices[i] - 1];
			vertex.normal = temp_normals[normalIndices[i] - 1];
			vertex.texUV = temp_uvs[uvIndices[i] - 1];
		}
		else if (objFormat == "noUv") {
			vertex.position = temp_vertices[vertexIndices[i] - 1];
			vertex.normal = temp_normals[normalIndices[i] - 1];
			vertex.texUV = glm::vec2(0., 0.);
		}
		else if (objFormat == "noUvNoNormals"){
			vertex.position = temp_vertices[vertexIndices[i] - 1];
			vertex.normal = glm::vec3(0., 0., 0.);
			vertex.texUV = glm::vec2(0., 0.);
		}

		vertices.push_back(vertex);
	}

	for (int i = 0; i < vertexIndices.size(); i++) {

		indices.push_back(i);
	}


	for (int i = 0; i < vertices.size(); i += 3) {

		glm::vec3 pos1 = vertices[i].position;
		glm::vec3 pos2 = vertices[i+1].position;
		glm::vec3 pos3 = vertices[i+2].position;

		glm::vec2 uv1 = vertices[i].texUV;
		glm::vec2 uv2 = vertices[i+1].texUV;
		glm::vec2 uv3 = vertices[i+2].texUV;

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;

		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		/*std::cout << "INDICES: " << std::endl;
		std::cout << vertexIndices[i] << " / " << vertexIndices[i+1] << " / " << vertexIndices[i+2] << std::endl;

		std::cout << "POS: " << std::endl;
		std::cout << pos1.x << " / " << pos1.y << " / " << pos1.z << std::endl;
		std::cout << pos2.x << " / " << pos2.y << " / " << pos2.z << std::endl;
		std::cout << pos3.x << " / " << pos3.y << " / " << pos3.z << std::endl;

		std::cout << "DATA: " << std::endl;
		std::cout << edge1.x << " / " << edge1.y << " / " << edge1.z << std::endl;
		std::cout << edge2.x << " / " << edge2.y << " / " << edge2.z << std::endl;
		std::cout << deltaUV1.x << " / " << deltaUV1.y << std::endl;
		std::cout << deltaUV2.x << " / " << deltaUV2.y << std::endl;*/

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		//std::cout << "F: " << f << std::endl;

		glm::vec3 tangent = glm::vec3(0.0f, 0.0f, 0.0f);

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, 0.0f);

		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		vertices[i].aTangent = tangent;
		vertices[i+1].aTangent = tangent;
		vertices[i+2].aTangent = tangent;

		vertices[i].aBitangent = bitangent;
		vertices[i+1].aBitangent = bitangent;
		vertices[i+2].aBitangent = bitangent;

		//std::cout << tangent.x << " / " << tangent.y << " / " << tangent.z << std::endl;
	}

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));
	VAO.LinkAttrib(VBO, 4, 3, GL_FLOAT, sizeof(Vertex), (void*)(11 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Delete()
{
	VAO.Delete();
}
