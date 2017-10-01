#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include <iostream>

#include "Buffer.hpp"

SlaveBuffer::SlaveBuffer(GLenum type, int memorySize, void* data = nullptr)
{
	Generate(type, memorySize, data);
}

void SlaveBuffer::Generate(GLenum type, int memorySize, void* data = nullptr)
{
	type_ = type;

	glGenBuffers(1, &key_);
	glBindBuffer(type_, key_);
	glBufferData(type_, memorySize, data, GL_STATIC_DRAW);
	glBindBuffer(type_, 0);
}

void SlaveBuffer::UploadData(void* data, GLuint size)
{
	glBindBuffer(type_, key_);
	glBufferSubData(type_, 0, size, data);
	glBindBuffer(type_, 0);
}

void SlaveBuffer::Bind()
{
	glBindBuffer(type_, key_);
}

void SlaveBuffer::Bind(GLuint bindPoint)
{
	glBindBuffer(type_, key_);
	if(type_ == GL_SHADER_STORAGE_BUFFER)
	{
		glBindBufferBase(type_, bindPoint, key_);
	}
}

void SlaveBuffer::Unbind()
{
	glBindBuffer(type_, 0);
}


MasterBuffer::MasterBuffer(int count)
{
	if(count != 0)
	{
		slaveBuffers_.Initialize(count);
	}

	Generate();
}

void MasterBuffer::Generate()
{
	glGenVertexArrays(1, &key_);
}

void MasterBuffer::AddStorageBuffer(int index, SlaveBuffer* buffer, const char* bufferName)
{
	if(!buffer)
		return;

	if(buffer->GetType() != GL_SHADER_STORAGE_BUFFER)
	{
		std::cout<<"Buffer is not of Storage type.\n";
		return;
	}

	auto bufferPointer = slaveBuffers_.Allocate(ShortWord(bufferName));
	*bufferPointer = buffer;

	Bind();
	buffer->Bind();
	glEnableVertexAttribArray(index);
	Unbind();
	buffer->Unbind();
}

void MasterBuffer::AddBuffer(int index, SlaveBuffer* buffer, const char* bufferName, GLuint elementSize, bool isFloating)
{

	if(!buffer)
		return;

	if(buffer->GetType() == GL_SHADER_STORAGE_BUFFER)
	{
		std::cout<<"Buffer is of Storage type.\n";
		return;
	}

	auto bufferPointer = slaveBuffers_.Allocate(ShortWord(bufferName));
	*bufferPointer = buffer;

	Bind();
	buffer->Bind();

	glEnableVertexAttribArray(index);
	if(isFloating)
	{
		glVertexAttribPointer(index, elementSize, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	else
	{
		glVertexAttribIPointer(index, elementSize, GL_INT, 0, NULL);
	}

	Unbind();
	buffer->Unbind();
}

SlaveBuffer* MasterBuffer::GetBuffer(const char* bufferName)
{
	return *slaveBuffers_.Get(ShortWord(bufferName));
}

void MasterBuffer::UploadData(const char* bufferName, void* data, GLuint memorySize)
{
	auto buffer = GetBuffer(bufferName);
	if(buffer != nullptr)
	{
		buffer->UploadData(data, memorySize);
	}
}

void MasterBuffer::SetSlaveBindPoint(const char* bufferName, GLuint bindPoint)
{
	auto buffer = GetBuffer(bufferName);
	buffer->Bind(bindPoint);
}

void MasterBuffer::Bind()
{
	glBindVertexArray(key_);
}

void MasterBuffer::Unbind()
{
	glBindVertexArray(0);
}

