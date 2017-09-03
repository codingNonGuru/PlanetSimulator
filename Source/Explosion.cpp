#include "Explosion.hpp"

void Explosion::Initialize(bool isPlayer, Mesh* mesh, Transform* transform, float impulse, bool hasDrag, bool isOrbiting)
{
	GameObject::Initialize(isPlayer, mesh, transform, impulse, hasDrag, isOrbiting);
	lifeTime_ = 0.0f;
}

void Explosion::updateLogic()
{
	lifeTime_ += 0.01f;
	if(lifeTime_ > 1.0f)
		isWorking_ = false;
}
