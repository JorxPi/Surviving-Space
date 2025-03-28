#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
	public:
		GLuint ID;
		Shader(const char* vertexFile, const char* fragmentFile);

		void Activate();
		void Delete();

		void SetMatrix4(const char* name, glm::mat4 data);
		void SetVector3f(const char* name, glm::vec3 data);
		void SetFloat(const char* name, float data);

		void SetTextureSampler(const char* name, GLint textureUnit);


};

#endif
