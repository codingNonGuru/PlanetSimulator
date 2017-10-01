#pragma once

#include <glm.hpp>
#include "Conventions.hpp"
#include "Memory.hpp"

class Transform;
class RigidBody;
typedef unsigned int GLuint;
class SlaveBuffer;
class MasterBuffer;
class Shader;
class ParticleSystem;

class ParticleSystem
{
	bool isValid_, isWorking_;
	int index_;
	float currentLifeTime_;
	float maximumLifeTime_;
	Transform* transform_;
	RigidBody* rigidBody_;
public:
	ParticleSystem() {}
	ParticleSystem(int, float, Transform*, RigidBody*);
	void Initialize(int, float, Transform*, RigidBody*);
	void Update();
	void SetValidity(bool isValid) {isValid_ = isValid;}
	bool IsValid() const {return isValid_;}
	bool IsWorking() const {return isWorking_;}
};

struct ParticleInitializer
{
	unsigned int systemIndex_;
	Position position_;

	ParticleInitializer() {}
	ParticleInitializer(unsigned int, Position);
	void Initialize(unsigned int, Position);
};

class ParticleSystemArray
{
	int systemSize_;
	int systemCount_;
	int particleCount_;
	int particleSize_;
	float systemLifeTime_;

	container::DynamicPool<ParticleSystem> particleSystems_;
	container::Array<ParticleInitializer> uploadQueue_;

	SlaveBuffer* particleBuffer_;
	SlaveBuffer* initializerBuffer_;
	MasterBuffer* renderBuffer_;

	Shader* startShader_;
	Shader* updateShader_;
	Shader* renderShader_;
public:
	void Initialize(int, int, float, int);
	void Update();
	void Allocate(Transform*, RigidBody*);
	void UploadQueue();
	void Render(Matrix&);
};
