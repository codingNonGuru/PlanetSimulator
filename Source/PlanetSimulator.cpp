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
#include "Ship.hpp"
#include "Structure.hpp"
#include "ParticleManager.hpp"

float elapsedTime = 0.0f;
GLuint planetVertexBuffer, planetCoordBuffer, planetVAO, postprocessVAO, particleVAO;
GLuint posSSBO, velSSBO, particleCount = 1024;
Scene mainScene;
Interface interface;

void Draw() {
	/*materialAtlas.use(Shaders::PARTICLES_COMPUTE);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velSSBO);
	glDispatchCompute(16384, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	materialAtlas.unuse(Shaders::PARTICLES_COMPUTE);*/

	Renderer::DrawScene(&mainScene);

	Renderer::DrawInterface(&interface);

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

	/*Renderer::GetMap()->use(Shaders::SPRITE);
	bindTexture(Shaders::SPRITE, "alpha", 0, Engine::sprites_[1].textureKey_);
	glUniform2f(2, Engine::sprites_[1].scale_.x * 0.25f, Engine::sprites_[1].scale_.y * 0.5f);
	glUniform1i(3, 0);
	for(Shell* shell = mainScene.shells_.getStart(); shell != mainScene.shells_.getEnd(); ++shell)
		if(shell->isValid_) {
			float speed = glm::length(shell->GetRigidBody()->velocity_);
			speed *= speed * 10.0f;
			glUniform1f(4, speed);
			shell->Draw(finalMatrix);
		}
	Renderer::GetMap()->unuse(Shaders::SPRITE);*/

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

	for(int i = 0; i < (int)Meshes::COUNT; ++i)
		Engine::meshes_.initialize((Meshes)i);

	Transform* transform = nullptr;
	RigidBody* rigidBody = nullptr;
	Ship* ship = nullptr;
	HealthBar* healthBar = nullptr;

	ShipFactory::SetScene(&mainScene);
	ShipFactory::SetInterface(&interface);

	//transform = new Transform(Position(100.0f, 5.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), 1.2f);
	//ship = ShipFactory::Produce(false, ShipTypes::SCOUT, transform);

	Planet* planet;
	planet = mainScene.planets_.allocate();
	transform = Transform::Allocate(&mainScene, Position(0.0f, 0.0f), Rotation(0.0f), 25.0f);
	rigidBody = RigidBody::Allocate(&mainScene, planet, 1.0f, 1.0f);
	planet->Initialize(false, &Engine::meshes_[Meshes::QUAD], transform, rigidBody, nullptr);
	planet->GetRigidBody()->angularMomentum_ = 0.0002f;
	planet->GetRigidBody()->angularDrag_ = 1.0f;

	Asteroid* asteroid;
	for(int i = 0; i < 40; ++i)
	{
		asteroid = mainScene.asteroids_.allocate();
		float angle = utility::getRandom(0.0f, 6.2831f);
		float radius = utility::biasedRandom(240.0f, 320.0f, 280.0f, 30.0f);
		auto position = Position(cos(angle) * radius, sin(angle) * radius);
		float size = utility::biasedRandom(15.0f, 40.0f, 15.0f, 5.0f);
		transform = Transform::Allocate(&mainScene, position, Rotation(utility::getRandom(0.0f, 6.2831f)), size);
		rigidBody = RigidBody::Allocate(&mainScene, asteroid, 100.0f, 1.0f);
		asteroid->Initialize(false, &Engine::meshes_[Meshes::QUAD], transform, rigidBody, nullptr);
		rigidBody->AddOrbitalVelocity(planet);
		float spin = utility::getRandom(-0.3f, 0.3f);
		asteroid->GetRigidBody()->Spin(spin);
		asteroid->GetRigidBody()->angularDrag_ = 1.0f;
		asteroid->GetTransform()->scale_ = utility::biasedRandom(1.0f, 2.5f, 1.0f, 0.5f);
	}

	Structure* outpost;
	outpost = mainScene.structures_.allocate();
	transform = Transform::Allocate(&mainScene, Position(30.0f, 0.0f), Rotation(0.0f), 6.0f);
	outpost->Initialize(false, &Engine::meshes_[Meshes::DOCK], transform, rigidBody, nullptr);
	outpost->SetPivot(planet);
	outpost->SetType(StructureTypes::OUTPOST);

	Structure* dock;
	dock = mainScene.structures_.allocate();
	transform = Transform::Allocate(&mainScene, Position(6.0f, 0.0f), Rotation(0.0f), 0.0f);
	dock->Initialize(false, &Engine::meshes_[Meshes::DOCK], transform, rigidBody, nullptr);
	dock->SetPivot(outpost);
	dock->SetType(StructureTypes::DOCK);

	transform = Transform::Allocate(&mainScene, Position(45.0f, 0.0f), Rotation(0.0f), 0.3f);
	ship = ShipFactory::Produce(true, ShipTypes::CORVETTE, transform);
	mainScene.ownShip_ = ship;

	for(int i = 0; i < 8; ++i)
	{
		transform = Transform::Allocate(&mainScene, Position(5.0f, (float(i) - 2.5f) * 0.01f), Rotation(0.0f), 0.3f);
		ship = ShipFactory::Produce(false, ShipTypes::BARGE, transform);
		ship->SetOrigin(outpost);
		ship->Attach(outpost);
		outpost->AddShip(ship);
	}

	Renderer::Initialize(&mainScene);

	/*GLuint key;
	glGenVertexArrays(1, &particleVAO);
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
	mainScene.Initialize();
	GameObject::mainScene_ = &mainScene;

	interface.Initialize();

	initializeGraphics();
}

int main(int argc, char* argv[]) {
	//double value = atof(argv[1]);
	//Renderer::SetZoomFactor(value);

	initializeSystem();
	EventHandler::Initialize();
	//ParticleManager::Initialize();

	while(Engine::IsRunning()){
		EventHandler::update();

		mainScene.UpdateCollisions();
		mainScene.UpdateLogic();

		//ParticleManager::FlushQueue();
		//ParticleManager::Update();

		for(int i = 0; i < 1; ++i)
			mainScene.UpdatePhysics();

		Draw();

		for(Shell* projectile = mainScene.shells_.getStart(); projectile != mainScene.shells_.getEnd(); ++projectile)
			if(projectile->isValid_ && !projectile->isWorking_) {
				projectile->Destroy();
				mainScene.shells_.deallocate(projectile);
			}
		for(Explosion* explosion = mainScene.explosions_.getStart(); explosion != mainScene.explosions_.getEnd(); ++explosion)
			if(explosion->isValid_ && !explosion->isWorking_) {
				explosion->Destroy();
				mainScene.explosions_.deallocate(explosion);
			}
		for(Ship* ship = mainScene.ships_.getStart(); ship != mainScene.ships_.getEnd(); ++ship)
			if(ship->isValid_ && !ship->isWorking_) {
				ship->Destroy();
				mainScene.ships_.deallocate(ship);
			}
		interface.CleanUp();

		SDL_GL_SwapWindow(Engine::window_);
	}
	return 0;
}
