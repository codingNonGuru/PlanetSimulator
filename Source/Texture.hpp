#pragma once

#include <glm.hpp>

#include "Memory.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Shader;

class Texture {
	GLuint key_;
	glm::vec2 size_;

public:
	Texture();

	virtual ~Texture();

	void Initialize(GLuint, glm::vec2);

	void Bind(GLuint, Shader*, const char*);

	void Unbind();

	void Upload(container::Container<>*, GLenum, GLenum, GLenum);

	void Delete();

	glm::vec2 GetSize() const {return size_;}
};


