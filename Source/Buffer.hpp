#pragma once

#include "Memory.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class SlaveBuffer;

class Buffer {
public:
	GLuint key_;

	Buffer() {}
};

class MasterBuffer : public Buffer
{
	container::StaticMap <SlaveBuffer*> slaveBuffers_;

public:
	MasterBuffer(int);
	void Generate();
	void AddBuffer(int, SlaveBuffer*, const char*, GLuint, bool);
	void AddStorageBuffer(int, SlaveBuffer*, const char*);
	SlaveBuffer* GetBuffer(int);
	SlaveBuffer* GetBuffer(const char*);
	void SetSlaveBindPoint(const char*, GLuint);
	void UploadData(const char*, void*, GLuint);
	void Bind();
	void Unbind();
};

class SlaveBuffer : public Buffer
{
	GLenum type_;
	unsigned int size_;

public:
	SlaveBuffer(GLenum, int, void*);
	GLenum GetType() const {return type_;}
	void Generate(GLenum, int, void*);
	void UploadData(void*, GLuint);
	void Bind();
	void Bind(GLuint);
	void Unbind();
	void SetSize(unsigned int size) {size_ = size;}
	unsigned int GetSize() const {return size_;}
};
