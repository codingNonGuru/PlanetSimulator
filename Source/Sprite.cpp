#include <stdlib.h>
#include "GL/glew.h"

#include "Sprite.hpp"

Sprite::Sprite() {}

Sprite::Sprite(glm::vec2 position) : position_(position) {
	hasContent_ = false;
}

Sprite::Sprite(glm::vec2 position, container::Container<> *buffer, GLenum internalFormat, GLenum format, GLenum type) {
	position_ = position;
	scale_ = glm::vec2(float(buffer->getWidth()), float(buffer->getHeight()));
	uploadTexture(buffer, internalFormat, format, type);
	hasContent_ = true;
}

void Sprite::initialize(container::Container<> *buffer, GLenum internalFormat, GLenum format, GLenum type) {
	if(hasContent_ == true) {
		deleteTexture();
	}
	scale_ = glm::vec2(float(buffer->getWidth()), float(buffer->getHeight()));
	uploadTexture(buffer, internalFormat, format, type);
}

void Sprite::uploadTexture(container::Container<> *buffer, GLenum internalFormat, GLenum format, GLenum type) {
	glGenTextures(1, &textureKey_);
	glBindTexture(GL_TEXTURE_2D, textureKey_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, buffer->getWidth(), buffer->getHeight(), 0, format, type, buffer->getVoidStart());
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::deleteTexture() {
	glBindTexture(GL_TEXTURE_2D, textureKey_);
	glDeleteTextures(1, &textureKey_);
}
