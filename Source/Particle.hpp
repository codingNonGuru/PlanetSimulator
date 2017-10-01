#pragma once

#include <glm.hpp>
#include "Conventions.hpp"

struct Particle
{
	Position position_;
	Direction velocity_;
	float lifeTime_;
	float decayRate_;

	void Initialize(Position position, Direction velocity, float lifeTime, float decayRate)
	{
		position_ = position;
		velocity_ = velocity;
		lifeTime_ = lifeTime;
		decayRate_ = decayRate;
	}
};
