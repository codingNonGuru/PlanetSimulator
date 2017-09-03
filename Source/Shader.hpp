#pragma once
#include <map>
#include <string>
typedef unsigned int GLuint;
typedef unsigned int GLenum;

enum class Shaders {
	SPRITE,
	POSTPROCESS,
	PARTICLES_COMPUTE,
	PARTICLES_INSTANCED,
	MESH,
	BODY,
	EXPLOSION,
	HEALTH_BAR,
	COUNT
};

enum class ShaderTypes {
	RENDER,
	COMPUTE
};

class Shader {
public:
	GLuint shaderKeys_[3];
	const char* shaderPaths_[3];
	unsigned int shaderCount_;
	GLuint key_;

	void initialize(const char*);
	void initialize(const char*, const char*);
	void initialize(const char*, const char*, const char*);
	Shader() {}
	Shader(const char*, const char*);
	~Shader(void);
	void use();
	void unuse();
	void compileShader(int, GLenum);
	void linkShaders();
	void addAttribute(const char*);
	void updateShaders();
	GLuint GetTextureLocation(const char*);
};

class ShaderMap {
private:
	Shader shaders_[(int)Shaders::COUNT];

public:
	bool initialize(Shaders, const char*, const char*, const char*);
	void use(Shaders material);
	void unuse(Shaders material);
	GLuint getTextureLocation(Shaders, const char*);
	Shader& get(Shaders material);
	Shader& operator [](Shaders material);
};


