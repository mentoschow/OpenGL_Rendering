#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace std;

class Shaders
{
public:
	GLuint ID;
	Shaders(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const string& name, bool value);
	void setInt(const string& name, int value);
	void setFloat(const string& name, float value);
	void setMat4(const string& name, const glm::mat4& mat);
	void setVec3(const string& name, float x, float y, float z);

	void reloadShader(GLuint* ID, const char* vertexPath, const char* fragmentPath);

private:
	void checkCompileErrors(unsigned int shader, string type);
	GLuint createShader(const char* vertexPath, const char* fragmentPath);
};

