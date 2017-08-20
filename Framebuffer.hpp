#pragma once

typedef unsigned int GLenum;
class Shader;

enum class Framebuffers {
	INITIAL,
	POSTPROCESS,
	DEFAULT,
	COUNT
};

enum class FramebufferTypes {
	MULTISAMPLE,
	SINGLESAMPLE,
	TEXTURE,
	RENDERBUFFER
};

class Framebuffer {
public:
    int width_, height_;
    unsigned int bufferKey_, colorKey_, depthKey_, depthBufferKey_;
    GLenum textureType_;

    Framebuffer();

    void initialize(FramebufferTypes, FramebufferTypes, int, int, bool);

    void bindBuffer(GLenum, GLenum);

    void unbindBuffer(GLenum);

    void bindTexture(Shader*, const char*, unsigned int);

    void unbindTexture();
};

class FramebufferAtlas {
private:
	Framebuffer framebuffers_[(int)Framebuffers::COUNT];

public:
	bool initialize(Framebuffers, FramebufferTypes, FramebufferTypes, int, int, bool);
	void use(Framebuffers, GLenum);
	void unuse(Framebuffers, GLenum);
	Framebuffer& get(Framebuffers);
	Framebuffer& operator [](Framebuffers);
};


