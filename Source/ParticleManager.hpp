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

class ParticleManager
{
	static Array<ParticleSystemArray> particleSystemArrays_;
public:
	static void Initialize();
	static void Allocate(ParticleSystems, Transform*, RigidBody*);
	static void Update();
	static void FlushQueue();
	static void Render(Matrix&);
};
