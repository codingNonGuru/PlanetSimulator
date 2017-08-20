/*
 * Engine.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include "Memory.hpp"
#include "Mesh.hpp"

class GLFWwindow;
typedef unsigned int GLuint;
class Sprite;
class SDL_Window;

struct Screen {
	int widthInteger_, heightInteger_;
	float widthFloating_, heightFloating_;

	void setAttributes(int width, int height) {
		widthInteger_ = width;
		heightInteger_ = height;
		widthFloating_ = (float)width;
		heightFloating_ = (float)height;
	}

	int getWidthInteger() const {
		return widthInteger_;
	}

	int getHeightInteger() const {
		return heightInteger_;
	}

	float getWidthFloating() const {
		return widthFloating_;
	}

	float getHeightFloating() const {
		return heightFloating_;
	}
};

class Engine {
public:
	static GLuint sampleCount_;
	static SDL_Window* window_;
	static Screen* screen_;
	static MeshAtlas meshes_;
	static Sprite* sprites_;
	static bool isRunning_;

	static void initialize(GLuint);
	static void ShutDown();
	static bool IsRunning() {return isRunning_;}
	Engine();
	virtual ~Engine();
};

#endif /* ENGINE_HPP_ */
