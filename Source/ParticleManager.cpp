#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "ParticleManager.hpp"
#include "ParticleSystem.hpp"
#include "Particle.hpp"

Array<ParticleSystemArray> ParticleManager::particleSystemArrays_ = Array<ParticleSystemArray>();

void ParticleManager::Initialize()
{
	particleSystemArrays_.initialize((int)ParticleSystems::COUNT);

	auto systemArray = particleSystemArrays_.get((int)ParticleSystems::EXPLOSION);
	systemArray->Initialize(256, 64, 5.0f, sizeof(Particle));
}

void ParticleManager::Allocate(ParticleSystems type, Transform* transform, RigidBody* rigidBody)
{
	int index = (int)type;
	auto systemArray = particleSystemArrays_.get(index);
	systemArray->Allocate(transform, rigidBody);
}

void ParticleManager::Update()
{
	auto systemArray = particleSystemArrays_.get((int)ParticleSystems::EXPLOSION);
	systemArray->Update();
}

void ParticleManager::FlushQueue()
{
	auto systemArray = particleSystemArrays_.get((int)ParticleSystems::EXPLOSION);
	systemArray->UploadQueue();
}

void ParticleManager::Render(Matrix& matrix)
{
	auto systemArray = particleSystemArrays_.get((int)ParticleSystems::EXPLOSION);
	systemArray->Render(matrix);
}
