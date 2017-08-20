#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include "Framebuffer.hpp"
#include "Engine.hpp"
#include "Shader.hpp"

Framebuffer::Framebuffer() {}

void Framebuffer::initialize(FramebufferTypes sampleType, FramebufferTypes storageType, int width, int height, bool isDefault) {
    width_ = width;
    height_ = height;

    if(isDefault == false) {
    	if(sampleType == FramebufferTypes::SINGLESAMPLE)
			textureType_ = GL_TEXTURE_2D;
		else
			textureType_ = GL_TEXTURE_2D_MULTISAMPLE;

    	glGenFramebuffers(1, &bufferKey_);
    	glBindFramebuffer(GL_FRAMEBUFFER, bufferKey_);

    	glGenTextures(1, &colorKey_);
    	glBindTexture(textureType_, colorKey_);
        if(sampleType == FramebufferTypes::MULTISAMPLE) {
        	glTexImage2DMultisample(textureType_, Engine::sampleCount_, GL_RGBA8, width_, height_, GL_FALSE);
        	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType_, colorKey_, 0);
        }
        else {
        	glTexImage2D(textureType_, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType_, colorKey_, 0);
        }

		glGenTextures(1, &depthKey_);
		glBindTexture(textureType_, depthKey_);
		if(sampleType == FramebufferTypes::MULTISAMPLE) {
			glTexImage2DMultisample(textureType_, Engine::sampleCount_, GL_DEPTH_COMPONENT32F, width_, height_, GL_FALSE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureType_, depthKey_, 0);
		}
		else {
			glTexImage2D(textureType_, 0, GL_DEPTH_COMPONENT32F, width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureType_, depthKey_, 0);
		}

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(textureType_, 0);
    }
    else {
        bufferKey_ = 0;
    }
}

void Framebuffer::bindBuffer(GLenum target, GLenum attachment) {
	glBindFramebuffer(target, bufferKey_);
	if(target == GL_DRAW_FRAMEBUFFER)
		glDrawBuffer(attachment);
	else
		glReadBuffer(attachment);
    glViewport(0, 0, width_, height_);
}

void Framebuffer::unbindBuffer(GLenum target) {
	glBindFramebuffer(GL_DRAW_BUFFER, 0);
    glBindFramebuffer(GL_READ_BUFFER, bufferKey_);

}

void Framebuffer::bindTexture(Shader* material, const char* textureName, unsigned int index) {
    GLuint firstWaveMapLocation = glGetUniformLocation(material->key_, textureName);
    glUniform1i(firstWaveMapLocation, index);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(textureType_, colorKey_);
}

void Framebuffer::unbindTexture() {
    glBindTexture(textureType_, 0);
}

bool FramebufferAtlas::initialize(Framebuffers framebuffer, FramebufferTypes sampleType, FramebufferTypes storageType, int width, int height, bool isDefault) {
	framebuffers_[(int)framebuffer].initialize(sampleType, storageType, width, height, isDefault);
	return true;
}

void FramebufferAtlas::use(Framebuffers framebuffer, GLenum target) {
	framebuffers_[(int)framebuffer].bindBuffer(target, GL_BACK);
}

void FramebufferAtlas::unuse(Framebuffers framebuffer, GLenum target) {
	framebuffers_[(int)framebuffer].unbindBuffer(target);
}

Framebuffer& FramebufferAtlas::get(Framebuffers framebuffer) {
	return framebuffers_[(int)framebuffer];
}

Framebuffer& FramebufferAtlas::operator [](Framebuffers framebuffer) {
	return framebuffers_[(int)framebuffer];
}
