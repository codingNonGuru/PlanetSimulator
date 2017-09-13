#include "Explosion.hpp"

void Explosion::Initialize(bool isPlayer, Mesh* mesh, Transform* transform, RigidBody* rigidBody, Controller* controller = nullptr)
{
	GameObject::Initialize(isPlayer, mesh, transform, rigidBody, controller);
	lifeTime_ = 0.0f;
}

void Explosion::updateLogic()
{
	lifeTime_ += 0.01f;
	if(lifeTime_ > 1.0f)
		isWorking_ = false;
}
