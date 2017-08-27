#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <ctime>

#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include <gtc/matrix_transform.hpp>

#include "Engine.hpp"
#include "EventHandler.hpp"
#include "Spaceship.hpp"
#include "Planet.hpp"
#include "Framebuffer.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "RigidBody.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Renderer.hpp"
#include "Utility.hpp"
#include "Texture.hpp"

float elapsedTime = 0.0f;
Planet* planets = NULL;
int planetCount = 3;
GLuint planetVertexBuffer, planetCoordBuffer, planetVAO, postprocessVAO, particleVAO;
FramebufferAtlas framebuffers;
GLuint posSSBO, velSSBO, particleCount = 1024;
int frames = 0;
Scene mainScene;

void bindTexture(Shaders shader, const char* name, unsigned int index, unsigned int textureKey) {
	glUniform1i(Renderer::GetMap()->getTextureLocation(shader, name), index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, textureKey);
}

void draw() {
	float zoomFactor = Renderer::GetZoomFactor();
	Spaceship* ownShip = mainScene.ownShip_;
	glm::vec3 screenCenter(ownShip->transform_->position_.x + -Engine::screen_->getWidthFloating() * 0.5f * zoomFactor, ownShip->transform_->position_.y + -Engine::screen_->getHeightFloating() * 0.5f * zoomFactor, 0.0f);
	glm::mat4 projectionMatrix = glm::ortho<float> (0.0f, Engine::screen_->getWidthFloating() * zoomFactor, Engine::screen_->getHeightFloating() * zoomFactor, 0.0f, 0.1f, 10.0f);
	glm::mat4 viewMatrix = glm::lookAt<float> (screenCenter + glm::vec3(0.0f, 0.0f, 1.0f), screenCenter, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 finalMatrix = projectionMatrix * viewMatrix;

	/*materialAtlas.use(Shaders::PARTICLES_COMPUTE);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velSSBO);
	glDispatchCompute(16384, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	materialAtlas.unuse(Shaders::PARTICLES_COMPUTE);*/

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_SHADING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	framebuffers[Framebuffers::DEFAULT].bindBuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0);
	//framebuffers[Framebuffers::DEFAULT].bindBuffer(GL_READ_BUFFER, GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClearDepth(1.0f);

	Renderer::Draw(&mainScene);

	Renderer::GetMap()->use(Shaders::SPRITE);
	bindTexture(Shaders::SPRITE, "alpha", 0, Engine::sprites_[2].textureKey_);
	glUniform2f(2, Engine::sprites_[2].scale_.x, Engine::sprites_[2].scale_.y);
	for(Planet* planet = mainScene.planets_.getStart(); planet != mainScene.planets_.getEnd(); ++planet)
		if(planet->isValid_) {
			glUniform1i(3, 0);
			planet->draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::SPRITE);

	Renderer::GetMap()->use(Shaders::SPRITE);
	//Renderer::perlinTexture_->Bind(0, &Renderer::GetMap()->get(Shaders::SPRITE), "alpha");
	bindTexture(Shaders::SPRITE, "alpha", 0, Engine::sprites_[0].textureKey_);
	glUniform2f(2, Engine::sprites_[0].scale_.x * 0.5f, Engine::sprites_[0].scale_.y * 0.5f);
	glUniform1i(3, 0);
	for(Spaceship* ship = mainScene.ships_.getStart(); ship != mainScene.ships_.getEnd(); ++ship)
		if(ship->isValid_) {
			glUniform1f(4, 1.0f);
			ship->draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::SPRITE);

	Renderer::GetMap()->use(Shaders::SPRITE);
	bindTexture(Shaders::SPRITE, "alpha", 0, Engine::sprites_[1].textureKey_);
	glUniform2f(2, Engine::sprites_[1].scale_.x, Engine::sprites_[1].scale_.y);
	glUniform1i(3, 0);
	for(Projectile* projectile = mainScene.projectiles_.getStart(); projectile != mainScene.projectiles_.getEnd(); ++projectile)
		if(projectile->isValid_) {
			float speed = glm::length(projectile->GetRigidBody()->velocity_) * 2.0f;
			if(speed > 1.0f)
				speed = 1.0f;
			glUniform1f(4, 3.0f * speed * speed - 2.0f * speed * speed * speed);
			projectile->draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::SPRITE);

	/*materialAtlas.use(Shaders::PARTICLES_INSTANCED);
	glBindVertexArray(particleVAO);
	glUniformMatrix4fv(0, 1, GL_FALSE, &finalMatrix[0][0]);
	glUniform1f(1, 10.0f);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particleCount + 1);
	glBindVertexArray(0);
	materialAtlas.unuse(Shaders::PARTICLES_INSTANCED);*/

	/*framebuffers[Framebuffers::DEFAULT].bindBuffer(GL_DRAW_FRAMEBUFFER, GL_BACK);
	framebuffers[Framebuffers::INITIAL].bindBuffer(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, Engine::screen_->getWidthInteger(), Engine::screen_->getHeightInteger(), 0, 0, Engine::screen_->getWidthInteger(), Engine::screen_->getHeightInteger(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

	framebuffers[Framebuffers::POSTPROCESS].bindBuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.0f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	framebuffers[Framebuffers::DEFAULT].bindBuffer(GL_READ_FRAMEBUFFER, GL_BACK);
	glBlitFramebuffer(0, 0, Engine::screen_->getWidthInteger(), Engine::screen_->getHeightInteger(), 0, 0, Engine::screen_->getWidthInteger(), Engine::screen_->getHeightInteger(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

	framebuffers[Framebuffers::DEFAULT].bindBuffer(GL_DRAW_FRAMEBUFFER, GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.0f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	materialAtlas.use(Shaders::POSTPROCESS);
	glBindVertexArray(postprocessVAO);
	glUniform2f(0, Engine::screen_->getWidthFloating(), Engine::screen_->getHeightFloating());
	framebuffers[Framebuffers::POSTPROCESS].bindTexture(&materialAtlas[Shaders::POSTPROCESS], "rawFrame", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	framebuffers[Framebuffers::POSTPROCESS].unbindTexture();
	glBindVertexArray(0);
	materialAtlas.unuse(Shaders::POSTPROCESS);*/
}

void initializeGraphics() {
	Engine::initialize(16);

	framebuffers.initialize(
		Framebuffers::INITIAL,
		FramebufferTypes::MULTISAMPLE,
		FramebufferTypes::TEXTURE,
		Engine::screen_->getWidthInteger(),
		Engine::screen_->getHeightInteger(),
		false
		);
	framebuffers.initialize(
		Framebuffers::POSTPROCESS,
		FramebufferTypes::SINGLESAMPLE,
		FramebufferTypes::RENDERBUFFER,
		Engine::screen_->getWidthInteger(),
		Engine::screen_->getHeightInteger(),
		false
		);
	framebuffers.initialize(
		Framebuffers::DEFAULT,
		FramebufferTypes::SINGLESAMPLE,
		FramebufferTypes::RENDERBUFFER,
		Engine::screen_->getWidthInteger(),
		Engine::screen_->getHeightInteger(),
		true
		);

	Engine::meshes_.initialize(Meshes::GENERIC_QUAD);
	Engine::meshes_.initialize(Meshes::SPACESHIP);
	Engine::meshes_.initialize(Meshes::PROJECTILE);

	Spaceship* ship;
	ship = mainScene.ships_.allocate();
	ship->initialize(true, &Engine::meshes_[Meshes::GENERIC_QUAD], glm::vec3(45.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, true, false);
	mainScene.ownShip_ = ship;
	ship = mainScene.ships_.allocate();
	ship->initialize(false, &Engine::meshes_[Meshes::GENERIC_QUAD], glm::vec3(70.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, true, false);

	Asteroid* asteroid;
	for(int i = 0; i < 80; ++i)
	{
		asteroid = mainScene.asteroids_.allocate();
		float angle = utility::getRandom(0.0f, 6.2831f);
		float radius = utility::biasedRandom(240.0f, 320.0f, 280.0f, 30.0f);//utility::getRandom(80.0f, 120.0f);
		asteroid->initialize(false, &Engine::meshes_[Meshes::GENERIC_QUAD], glm::vec3(cos(angle) * radius, sin(angle) * radius, 0.0f), glm::vec3(0.0f, 0.0f, utility::getRandom(0.0f, 6.2831f)), 0.0f, true, true);
		asteroid->GetRigidBody()->angularMomentum_ = 0.01f;
		asteroid->GetRigidBody()->angularDrag_ = 1.0f;
		asteroid->GetTransform()->scale_ = utility::biasedRandom(1.0f, 2.5f, 1.0f, 0.5f);
	}

	Planet* planet;
	planet = mainScene.planets_.allocate();
	planet->initialize(false, &Engine::meshes_[Meshes::GENERIC_QUAD], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, true, false);
	planet->GetRigidBody()->angularMomentum_ = 0.001f;
	planet->GetRigidBody()->angularDrag_ = 1.0f;

	GLuint key;
	/*glGenVertexArrays(1, &postprocessVAO);
	glBindVertexArray(postprocessVAO);
	glGenBuffers(1, &key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 6, &Planet::vertices_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	Renderer::Initialize(&mainScene);

	/*glGenVertexArrays(1, &particleVAO);
	glBindVertexArray(particleVAO);
	glGenBuffers(1, &key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 6, &Planet::vertices_[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, posSSBO);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(4, 1);
	glBindBuffer(GL_ARRAY_BUFFER, velSSBO);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(5, 1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

void initializeSystem() {
#ifdef GRAVITY
	printf("Gravity value is: %i!\n", GRAVITY_VALUE);
#endif
	mainScene.initialize();
	GameObject::mainScene_ = &mainScene;

	initializeGraphics();
}

int main() {
	initializeSystem();
	EventHandler::Initialize();

	while(Engine::IsRunning()){
		EventHandler::update();

		for(Spaceship* ship = mainScene.ships_.getStart(); ship != mainScene.ships_.getEnd(); ++ship)
		{
			if(ship->isValid_ && ship->isWorking_)
			{
				ship->updateLogic();
			}
		}
		for(auto projectile = mainScene.projectiles_.getStart(); projectile != mainScene.projectiles_.getEnd(); ++projectile)
			if(projectile->isValid_ && projectile->isWorking_) {
				projectile->updateLogic();
			}

		for(int i = 0; i < 2; ++i)
			mainScene.UpdatePhysics();

		draw();

		for(auto projectile = mainScene.projectiles_.getStart(); projectile != mainScene.projectiles_.getEnd(); ++projectile)
			if(projectile->isValid_ == true && projectile->isWorking_ == false) {
				projectile->isValid_ = false;
				mainScene.projectiles_.deallocate(projectile);
			}
		SDL_GL_SwapWindow(Engine::window_);
	}
	return 0;
}
