#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "ParticleManager.hpp"
#include "ParticleSystem.hpp"
#include "Particle.hpp"
#include "Buffer.hpp"
#include "Utility.hpp"
#include "Fractal.hpp"
#include "Texture.hpp"

Array<ParticleSystemArray> ParticleManager::particleSystemArrays_ = Array<ParticleSystemArray>();
SlaveBuffer* ParticleManager::randomStreamBuffer_ = nullptr;
Texture ParticleManager::distortionMap_ = Texture();

void ParticleManager::Initialize()
{
	Array<float> randomStream(1024 * 1024);
	for(int i = 0; i < randomStream.getCapacity(); ++i)
	{
		*randomStream.allocate() = utility::getRandom(0.0f, 1.0f);
	}
	randomStreamBuffer_ = new SlaveBuffer(GL_SHADER_STORAGE_BUFFER, randomStream.getMemorySize(), randomStream.getStart());
	randomStreamBuffer_->SetSize(randomStream.getSize());

	particleSystemArrays_.initialize((int)ParticleSystems::COUNT);

	auto systemArray = particleSystemArrays_.get((int)ParticleSystems::EXPLOSION);
	systemArray->Initialize(512, 64, 10.0f, sizeof(Particle));

	container::Grid<float> result(512, 512);
	Perlin::generate(true, result, Range(0.0f, 1.0f), 4.0f, 2.0f, 0.5f, 2.0f);
	distortionMap_.Upload(&result, GL_R32F, GL_RED, GL_FLOAT);
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
