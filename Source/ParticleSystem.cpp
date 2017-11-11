#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "ParticleManager.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"
#include "ParticleSystem.hpp"
#include "Transform.hpp"
#include "Particle.hpp"
#include "Utility.hpp"
#include "Texture.hpp"

// PARTICLE SYSTEM

ParticleSystem::ParticleSystem(int index, float maximumLifeTime, Transform* transform, RigidBody* rigidBody)
{
	Initialize(index, maximumLifeTime, transform, rigidBody);
}

void ParticleSystem::Initialize(int index, float maximumLifeTime, Transform* transform, RigidBody* rigidBody)
{
	isValid_ = true;
	isWorking_ = true;
	index_ = index;
	currentLifeTime_ = 0.0f;
	maximumLifeTime_ = maximumLifeTime;
	transform_ = transform;
	rigidBody_ = rigidBody;
}

void ParticleSystem::Update()
{
	currentLifeTime_ += 0.1f;
	if(currentLifeTime_ > maximumLifeTime_)
	{
		isWorking_ = false;
	}
}

// PARTICLE INITIALIZER

ParticleInitializer::ParticleInitializer(unsigned int index, Position position)
{
	Initialize(index, position);
}

void ParticleInitializer::Initialize(unsigned int index, Position position)
{
	systemIndex_ = index;
	position_ = position;
}

// PARTICLE SYSTEM ARRAY

void ParticleSystemArray::Initialize(int systemSize, int systemCount, float systemLifeTime, int particleSize)
{
	systemSize_ = systemSize;
	systemCount_ = systemCount;
	particleCount_ = systemSize_ * systemCount_;
	particleSize_ = particleSize;
	systemLifeTime_ = systemLifeTime;

	particleSystems_.initialize(systemCount_);
	for(auto system = particleSystems_.getStart(); system != particleSystems_.getEnd(); ++system)
	{
		system->SetValidity(false);
	}
	uploadQueue_.initialize(systemCount_);

	Particle* particles = new Particle[particleCount_];
	for(auto particle = particles; particle != particles + particleCount_; ++particle)
	{
		float angle = utility::getRandom(0.0f, 6.2831f);
		float speed = utility::biasedRandom(0.2f, 1.0f, 0.3f, 0.15f);
		particle->Initialize(
			Position(utility::getRandom(-50.0f, 50.0f), utility::getRandom(-50.0f, 50.0f)),
			Direction(cos(angle) * speed, sin(angle) * speed),
			0.0f,
			utility::biasedRandom(0.2f, 1.0f, 0.8f, 0.1f)
			);
	}
	particleBuffer_ = new SlaveBuffer(GL_SHADER_STORAGE_BUFFER, particleSize_ * particleCount_, particles);
	initializerBuffer_ = new SlaveBuffer(GL_SHADER_STORAGE_BUFFER, systemCount_ * sizeof(ParticleInitializer), nullptr);
	renderBuffer_ = new MasterBuffer(4);
	renderBuffer_->AddStorageBuffer(0, particleBuffer_, "particle");

	startShader_ = new Shader("Shaders/ParticleStart.comp");
	updateShader_ = new Shader("Shaders/ParticleUpdate.comp");
	renderShader_ = new Shader("Shaders/Particle.vert", "Shaders/Particle.frag");
}

void ParticleSystemArray::Update()
{
	updateShader_->use();
	particleBuffer_->Bind(0);
	ParticleManager::GetDistortionMap()->Bind(0, updateShader_, "distortion");
	glUniform1ui(0, uploadQueue_.getSize());
	glUniform1ui(1, systemSize_);
	glDispatchCompute(particleCount_ / 16, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glFinish();
	particleBuffer_->Unbind();
	ParticleManager::GetDistortionMap()->Unbind();
	updateShader_->unuse();

	for(auto system = particleSystems_.getStart(); system != particleSystems_.getEnd(); ++system)
	{
		system->Update();
	}

	for(auto system = particleSystems_.getStart(); system != particleSystems_.getEnd(); ++system)
	{
		if(system->IsValid() && !system->IsWorking())
		{
			system->SetValidity(false);
			particleSystems_.deallocate(system);
		}
	}
}

void ParticleSystemArray::Allocate(Transform* transform, RigidBody* rigidBody)
{
	auto newSystem = particleSystems_.allocate();
	auto index = newSystem - particleSystems_.getStart();
	*newSystem = ParticleSystem(index, systemLifeTime_, transform, rigidBody);

	*uploadQueue_.allocate() = ParticleInitializer(index, transform->position_);
}

void ParticleSystemArray::UploadQueue()
{
	if(!initializerBuffer_ || !startShader_)
		return;

	if(uploadQueue_.getSize() == 0)
		return;

	for(auto init = uploadQueue_.getStart(); init != uploadQueue_.getEnd(); ++init)
	{
		std::cout<<init->systemIndex_<<" ";
	}
	std::cout<<"\n";

	initializerBuffer_->UploadData(uploadQueue_.getStart(), uploadQueue_.getMemorySize());

	startShader_->use();
	particleBuffer_->Bind(0);
	initializerBuffer_->Bind(1);
	auto randomStreamBuffer = ParticleManager::GetStreamBuffer();
	randomStreamBuffer->Bind(2);
	ParticleManager::GetDistortionMap()->Bind(0, startShader_, "distortion");
 	glUniform1ui(0, uploadQueue_.getSize());
	glUniform1ui(1, systemSize_);
	glUniform1ui(2, randomStreamBuffer->GetSize());
	glUniform1ui(3, utility::getRandom(0, 100000000));
	glUniform1ui(4, utility::getRandom(0, 100000000));
	glDispatchCompute(uploadQueue_.getSize() * systemSize_ / 16, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glFinish();
	particleBuffer_->Unbind();
	initializerBuffer_->Unbind();
	randomStreamBuffer->Unbind();
	ParticleManager::GetDistortionMap()->Unbind();
	startShader_->unuse();

	uploadQueue_.reset();
}

void ParticleSystemArray::Render(Matrix& matrix)
{
	if(!renderBuffer_ || !renderShader_)
		return;

	renderShader_->use();
	renderBuffer_->Bind();
	renderBuffer_->SetSlaveBindPoint("particle", 0);
	glUniformMatrix4fv(0, 1, GL_FALSE, &matrix[0][0]);
	glUniform1f(1, 0.2f);
	glDrawArrays(GL_TRIANGLES, 0, particleCount_ * 6);
	renderBuffer_->Unbind();
	renderShader_->unuse();
}
