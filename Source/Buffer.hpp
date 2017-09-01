#pragma once

#include "Memory.hpp"

typedef unsigned int GLuint;

class Buffer {
public:
	GLuint key_;
	container::Array<Buffer> subBuffers_;

	Buffer() {}
	Buffer(GLuint, int);
	void SetKey(GLuint key) {key_ = key;}
	GLuint GetKey() const {return key_;}
	GLuint GetKey(int index) const {Buffer* buffer = subBuffers_.get(index); return buffer->GetKey();}
	void AddBuffer(GLuint);
};
