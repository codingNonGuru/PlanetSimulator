/*
 * Engine.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */
#include <iostream>
#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Engine.hpp"
#include "Sprite.hpp"
#include "Fractal.hpp"

SDL_Window* Engine::window_ = nullptr;
Screen* Engine::screen_ = nullptr;
GLuint Engine::sampleCount_ = 0;
MeshAtlas Engine::meshes_ = MeshAtlas();
Sprite* Engine::sprites_ = nullptr;
bool Engine::isRunning_ = true;

void Engine::initialize(GLuint sampleCount) {
	isRunning_ = true;
	screen_ = new Screen();
	screen_->setAttributes(2560, 1440);
	sampleCount_ = sampleCount;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, sampleCount_ );
	window_ = SDL_CreateWindow("PlanetSimulator", 0, 0, screen_->getWidthInteger(), screen_->getHeightInteger(), SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
	SDL_GLContext context = SDL_GL_CreateContext(window_);
	SDL_GL_SetSwapInterval(1);
	SDL_GL_MakeCurrent(window_, context);

	glewInit();

	glViewport(0, 0, screen_->getWidthInteger(), screen_->getHeightInteger());

	struct ByteSizePixel {
		unsigned char r, g, b, a;
	};

	sprites_ = new Sprite[32];

	SDL_Surface* image = IMG_Load("Textures/triangle.png");
	container::Grid<uint8_t> texture(image->w, image->h);
	ByteSizePixel* spr = (ByteSizePixel*)image->pixels;
	for(auto tex = texture.getStart(); tex != texture.getEnd(); ++tex)
		*tex = 0.0f;
	for(int y = 0; y < image->h; ++y)
		for(int x = 0; x < image->w; ++x) {
			*texture(x, y) = spr->a;
			spr++;
		}
	sprites_[0] = Sprite(glm::vec2(0.0f, 0.0f), &texture, GL_R8, GL_RED, GL_UNSIGNED_BYTE);
	sprites_[0].scale_ = glm::vec2(image->w, image->h);

	image = IMG_Load("Textures/shell.png");
	texture.initialize(image->w, image->h);
	spr = (ByteSizePixel*)image->pixels;
	for(auto tex = texture.getStart(); tex != texture.getEnd(); ++tex)
		*tex = 0.0f;
	for(int y = 0; y < image->h; ++y)
		for(int x = 0; x < image->w; ++x) {
			*texture(x, y) = spr->a;
			spr++;
		}
	sprites_[1] = Sprite(glm::vec2(0.0f, 0.0f), &texture, GL_R8, GL_RED, GL_UNSIGNED_BYTE);
	sprites_[1].scale_ = glm::vec2(image->w, image->h);

	image = IMG_Load("Textures/asteroid.png");
	texture.initialize(image->w, image->h);
	spr = (ByteSizePixel*)image->pixels;
	for(auto tex = texture.getStart(); tex != texture.getEnd(); ++tex)
		*tex = 0.0f;
	for(int y = 0; y < image->h; ++y)
		for(int x = 0; x < image->w; ++x) {
			*texture(x, y) = spr->a;
			spr++;
		}
	sprites_[2] = Sprite(glm::vec2(0.0f, 0.0f), &texture, GL_R8, GL_RED, GL_UNSIGNED_BYTE);
	sprites_[2].scale_ = glm::vec2(image->w, image->h);

	Perlin::initialize();
}

void Engine::ShutDown() {
	isRunning_ = false;
	SDL_Quit();
}

Engine::Engine() {
	// TODO Auto-generated constructor stub

}

Engine::~Engine() {
	// TODO Auto-generated destructor stub
}

