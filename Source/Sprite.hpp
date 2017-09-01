#ifndef SOURCE_ENGINE_SPRITE_HPP_
#define SOURCE_ENGINE_SPRITE_HPP_

#include <glm.hpp>

#include "Memory.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;


class Sprite {
public:
	bool hasContent_;
	GLuint textureKey_;
	glm::vec2 position_;
	glm::vec2 scale_;

	Sprite();
	Sprite(glm::vec2);
	Sprite(glm::vec2, container::Container<>*, GLenum, GLenum, GLenum);
	void initialize(container::Container<>*, GLenum, GLenum, GLenum);
	void uploadTexture(container::Container<>*, GLenum, GLenum, GLenum);
	void deleteTexture();
};

#endif
