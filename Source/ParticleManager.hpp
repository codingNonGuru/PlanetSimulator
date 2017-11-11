#pragma once

#include <glm.hpp>
#include "Memory.hpp"
#include "Types.hpp"
#include "Conventions.hpp"

class ParticleSystem;
class ParticleSystemArray;
template <class Type>
using Array = container::Array<Type>;
template <class Type>
using Pool = container::DynamicPool<Type>;
class Transform;
class RigidBody;
class SlaveBuffer;
class Texture;

class ParticleManager
{
	static Array<ParticleSystemArray> particleSystemArrays_;
	static SlaveBuffer* randomStreamBuffer_;
	static Texture distortionMap_;
public:
	static void Initialize();
	static void Allocate(ParticleSystems, Transform*, RigidBody*);
	static void Update();
	static void FlushQueue();
	static void Render(Matrix&);
	static SlaveBuffer* GetStreamBuffer() {return randomStreamBuffer_;}
	static Texture* GetDistortionMap() {return &distortionMap_;}
};
