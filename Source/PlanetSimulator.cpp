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
#include "Explosion.hpp"
#include "Interface.hpp"
#include "HealthBar.hpp"
#include "Factory.hpp"

float elapsedTime = 0.0f;
Planet* planets = NULL;
int planetCount = 3;
GLuint planetVertexBuffer, planetCoordBuffer, planetVAO, postprocessVAO, particleVAO;
FramebufferAtlas framebuffers;
GLuint posSSBO, velSSBO, particleCount = 1024;
int frames = 0;
Scene mainScene;
Interface interface;

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
			planet->Draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::SPRITE);

	/*Renderer::GetMap()->use(Shaders::SPRITE);
	bindTexture(Shaders::SPRITE, "alpha", 0, Engine::sprites_[0].textureKey_);
	glUniform2f(2, Engine::sprites_[0].scale_.x * 0.5f, Engine::sprites_[0].scale_.y * 0.5f);
	glUniform1i(3, 0);
	for(Spaceship* ship = mainScene.ships_.getStart(); ship != mainScene.ships_.getEnd(); ++ship)
		if(ship->isValid_) {
			glUniform1f(4, 1.0f);
			ship->draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::SPRITE);*/

	Renderer::GetMap()->use(Shaders::MESH);
	for(Spaceship* ship = mainScene.ships_.getStart(); ship != mainScene.ships_.getEnd(); ++ship)
		if(ship->isValid_) {
			glUniform1f(2, ship->GetTransform()->scale_);
			ship->Draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::MESH);

	Renderer::GetMap()->use(Shaders::SPRITE);
	bindTexture(Shaders::SPRITE, "alpha", 0, Engine::sprites_[1].textureKey_);
	glUniform2f(2, Engine::sprites_[1].scale_.x * 0.5f, Engine::sprites_[1].scale_.y * 0.5f);
	glUniform1i(3, 0);
	for(Projectile* projectile = mainScene.projectiles_.getStart(); projectile != mainScene.projectiles_.getEnd(); ++projectile)
		if(projectile->isValid_ && projectile->isWorking_) {
			float speed = glm::length(projectile->GetRigidBody()->velocity_) * 2.5f;
			if(speed > 1.0f)
				speed = 1.0f;
			glUniform1f(4, 3.0f * speed * speed - 2.0f * speed * speed * speed);
			projectile->Draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::SPRITE);

	Renderer::GetMap()->use(Shaders::EXPLOSION);
	for(Explosion* explosion = mainScene.explosions_.getStart(); explosion != mainScene.explosions_.getEnd(); ++explosion)
		if(explosion->isValid_ && explosion->isWorking_) {
			glUniform1f(2, 5.0f);
			glUniform1f(3, explosion->lifeTime_);
			//glUniform1f(4, 1.0f);
			explosion->Draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::EXPLOSION);

	Renderer::GetMap()->use(Shaders::HEALTH_BAR);
	for(HealthBar* bar = interface.healthBars_.getStart(); bar != interface.healthBars_.getEnd(); ++bar)
		if(bar->IsValid() && bar->IsWorking()) {
			glUniform1f(2, 5.0f);
			glUniform1f(3, bar->GetShip()->hull_.GetDamage());
			glUniform1f(4, bar->GetShip()->weapon_->GetHeatFactor());
			bar->Draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::HEALTH_BAR);

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
	Engine::meshes_.initialize(Meshes::SPACESHIP_SCOUT);
	Engine::meshes_.initialize(Meshes::SPACESHIP_CORVETTE);
	Engine::meshes_.initialize(Meshes::PROJECTILE);

	Transform* transform = nullptr;
	RigidBody* rigidBody = nullptr;
	Spaceship* ship = nullptr;
	HealthBar* healthBar = nullptr;

	ShipFactory::SetScene(&mainScene);
	ShipFactory::SetInterface(&interface);

	transform = new Transform(Position(45.0f, 0.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), 1.2f);
	ship = ShipFactory::Produce(true, ShipTypes::CORVETTE, transform);
	mainScene.ownShip_ = ship;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			transform = new Transform(Position(100.0f + (float)i * 5.0f, 5.0f + (float)j * 5.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), 1.2f);
			ship = ShipFactory::Produce(false, ShipTypes::SCOUT, transform);
		}
	}

	//transform = new Transform(Position(100.0f, 5.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), 1.2f);
	//ship = ShipFactory::Produce(false, ShipTypes::SCOUT, transform);

	Planet* planet;
	planet = mainScene.planets_.allocate();
	transform = new Transform(Position(0.0f, 0.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), 20.0f);
	rigidBody = new RigidBody(planet, 1.0f, 1.0f);
	planet->Initialize(false, &Engine::meshes_[Meshes::GENERIC_QUAD], transform, rigidBody);
	planet->GetRigidBody()->angularMomentum_ = 0.001f;
	planet->GetRigidBody()->angularDrag_ = 1.0f;

	Asteroid* asteroid;
	for(int i = 0; i < 80; ++i)
	{
		asteroid = mainScene.asteroids_.allocate();
		float angle = utility::getRandom(0.0f, 6.2831f);
		float radius = utility::biasedRandom(240.0f, 320.0f, 280.0f, 30.0f);
		auto position = Position(cos(angle) * radius, sin(angle) * radius, 0.0f);
		transform = new Transform(position, Rotation(0.0f, 0.0f, utility::getRandom(0.0f, 6.2831f)), 20.0f);
		rigidBody = new RigidBody(asteroid, 1.0f, 1.0f);
		asteroid->Initialize(false, &Engine::meshes_[Meshes::GENERIC_QUAD], transform, rigidBody);
		rigidBody->AddOrbitalVelocity(planet);
		asteroid->GetRigidBody()->angularMomentum_ = 0.01f;
		asteroid->GetRigidBody()->angularDrag_ = 1.0f;
		asteroid->GetTransform()->scale_ = utility::biasedRandom(1.0f, 2.5f, 1.0f, 0.5f);
	}

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

	interface.Initialize();

	initializeGraphics();
}

int main() {
	initializeSystem();
	EventHandler::Initialize();

	while(Engine::IsRunning()){
		EventHandler::update();

		mainScene.UpdateCollisions();
		for(Spaceship* ship = mainScene.ships_.getStart(); ship != mainScene.ships_.getEnd(); ++ship)
		{
			if(ship->isValid_ && ship->isWorking_)
			{
				ship->updateLogic();
			}
		}
		for(Explosion* explosion = mainScene.explosions_.getStart(); explosion != mainScene.explosions_.getEnd(); ++explosion)
		{
			if(explosion->isValid_ && explosion->isWorking_)
			{
				explosion->updateLogic();
			}
		}
		for(auto projectile = mainScene.projectiles_.getStart(); projectile != mainScene.projectiles_.getEnd(); ++projectile)
			if(projectile->isValid_ && projectile->isWorking_) {
				projectile->updateLogic();
			}

		for(int i = 0; i < 1; ++i)
			mainScene.UpdatePhysics();

		draw();

		for(Projectile* projectile = mainScene.projectiles_.getStart(); projectile != mainScene.projectiles_.getEnd(); ++projectile)
			if(projectile->isValid_ && !projectile->isWorking_) {
				projectile->Destroy();
				mainScene.projectiles_.deallocate(projectile);
			}
		for(Explosion* explosion = mainScene.explosions_.getStart(); explosion != mainScene.explosions_.getEnd(); ++explosion)
			if(explosion->isValid_ && !explosion->isWorking_) {
				explosion->Destroy();
				mainScene.explosions_.deallocate(explosion);
			}
		for(Spaceship* ship = mainScene.ships_.getStart(); ship != mainScene.ships_.getEnd(); ++ship)
			if(ship->isValid_ && !ship->isWorking_) {
				ship->Destroy();
				mainScene.ships_.deallocate(ship);
			}
		interface.CleanUp();

		SDL_GL_SwapWindow(Engine::window_);
	}
	return 0;
}
