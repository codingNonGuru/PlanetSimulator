#include <stdlib.h>
#include "GL/glew.h"

#include "Texture.hpp"
#include "Shader.hpp"

Texture::Texture() {

}

Texture::~Texture() {

}

void Texture::Initialize(GLuint key, glm::vec2 size) {
	key_ = key;
	size_ = size;
}

void Texture::Bind(GLuint index, Shader* shader, const char* name)
{
	GLuint location = shader->GetTextureLocation(name);
	glUniform1i(location, index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, key_);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Upload(container::Container<> *buffer, GLenum internalFormat, GLenum format, GLenum type) {
	glGenTextures(1, &key_);
	glBindTexture(GL_TEXTURE_2D, key_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, buffer->getWidth(), buffer->getHeight(), 0, format, type, buffer->getVoidStart());
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);

	size_ = glm::vec2((float)buffer->getWidth(), (float)buffer->getHeight());
}

void Texture::Delete() {
	glBindTexture(GL_TEXTURE_2D, key_);
	glDeleteTextures(1, &key_);
}

