#include "Shader.hpp"

#include <stdlib.h>
#include "GL/glew.h"
//#include <gl/GL.h>
#include <fstream>
#include <iostream>
#include <vector>

bool ShaderMap::initialize(Shaders shader, const char* firstShaderPath, const char* secondShaderPath, const char* thirdShaderPath) {
	if(thirdShaderPath != nullptr)
		shaders_[(int)shader].initialize(firstShaderPath, secondShaderPath, thirdShaderPath);
	else if(secondShaderPath != nullptr)
		shaders_[(int)shader].initialize(firstShaderPath, secondShaderPath);
	else
		shaders_[(int)shader].initialize(firstShaderPath);

	return true;
}

void ShaderMap::use(Shaders material) {
	shaders_[(int)material].use();
}

void ShaderMap::unuse(Shaders material) {
	shaders_[(int)material].unuse();
}

GLuint ShaderMap::getTextureLocation(Shaders shader, const char* textureName) {
	return glGetUniformLocation(shaders_[(int)shader].key_, textureName);
}

Shader& ShaderMap::get(Shaders material) {
	return shaders_[(int)material];
}

Shader& ShaderMap::operator [](Shaders material) {
	return shaders_[(int)material];
}

void Shader::initialize(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath) {
	shaderPaths_[0] = vertexShaderPath;
	shaderPaths_[1] = fragmentShaderPath;
	shaderPaths_[2] = geometryShaderPath;
	shaderCount_ = 3;
	compileShader(0, GL_VERTEX_SHADER);
	compileShader(1, GL_FRAGMENT_SHADER);
	compileShader(2, GL_GEOMETRY_SHADER);
	linkShaders();
}

void Shader::initialize(const char* vertexShaderPath, const char* fragmentShaderPath) {
	shaderPaths_[0] = vertexShaderPath;
	shaderPaths_[1] = fragmentShaderPath;
	shaderCount_ = 2;
	compileShader(0, GL_VERTEX_SHADER);
	compileShader(1, GL_FRAGMENT_SHADER);
	linkShaders();
}

void Shader::initialize(const char* computeShaderPath) {
	shaderPaths_[0] = computeShaderPath;
	shaderCount_ = 1;
	compileShader(0, GL_COMPUTE_SHADER);
	linkShaders();
}


Shader::Shader(const char* vertShaderPath, const char* fragShaderPath) {
	/*vertString = vertShaderPath;
	fragString = fragShaderPath;
	compileShader(vertShaderPath, fragShaderPath);
	linkShaders();*/
}

Shader::~Shader(void) {
}

void Shader::use() {
	glUseProgram(key_);
}

void Shader::unuse() {
	glUseProgram(0);
}

void Shader::compileShader(int index, GLenum shaderType) {
	GLuint key = glCreateShader(shaderType);
	shaderKeys_[index] = key;

	std::ifstream shaderFile(shaderPaths_[index]);
	std::string fileContents = "";
	std::string Line;
	while(std::getline(shaderFile, Line))
		fileContents += Line + "\n";
	shaderFile.close();
	const char* fC = fileContents.c_str();
	glShaderSource(key, 1, &fC, nullptr);
	glCompileShader(key);

	GLint isCompiled = 0;
	glGetShaderiv(key, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(key, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(key, maxLength, &maxLength, &errorLog[0]);
		for(int i = 0; i < errorLog.size(); ++i)
			std::cout<<errorLog[i];
		glDeleteShader(key);
		return;
	}
}

void Shader::linkShaders() {
	key_ = glCreateProgram();

	for(int i = 0; i < shaderCount_; ++i) {
		glAttachShader(key_, shaderKeys_[i]);
	}

	glLinkProgram(key_);
	glValidateProgram(key_);

	GLint isLinked = 0;
	glGetProgramiv(key_, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(key_, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(key_, maxLength, &maxLength, &infoLog[0]);
		for(int i = 0; i < infoLog.size(); ++i)
			std::cout<<infoLog[i];
		glDeleteProgram(key_);
		for(int i = 0; i < shaderCount_; ++i)
			glDeleteShader(shaderKeys_[i]);
		return;
	}

	for(int i = 0; i < shaderCount_; ++i)
		glDetachShader(key_, shaderKeys_[i]);
}

void Shader::addAttribute(const char* attrName) {
	//glBindAttribLocation(ID, attrCount, attrName.c_str());
	//attrCount++;
}

void Shader::updateShaders() {
	glDeleteProgram(key_);
	for(int i = 0; i < shaderCount_; ++i)
		glDeleteShader(shaderKeys_[i]);
	if(shaderCount_ == 1) {
		compileShader(0, GL_COMPUTE_SHADER);
	}
	else if(shaderCount_ == 2) {
		compileShader(0, GL_VERTEX_SHADER);
		compileShader(1, GL_FRAGMENT_SHADER);
	} else if(shaderCount_ == 3) {
		compileShader(0, GL_VERTEX_SHADER);
		compileShader(1, GL_FRAGMENT_SHADER);
		compileShader(2, GL_GEOMETRY_SHADER);
	}
	linkShaders();
}

GLuint Shader::GetTextureLocation(const char* textureName) {
	return glGetUniformLocation(key_, textureName);
}
