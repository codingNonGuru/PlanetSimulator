#include "Collider.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

void Collider::Initialize(GameObject* parent, BoundingBoxes boundingBox)
{
	parent_ = parent;
	boundingBox_ = boundingBox;
	collision_ = nullptr;
}

void Collider::Update()
{
	Scene* scene = GameObject::mainScene_;

	if(!scene)
		return;

	if(collision_)
	{
		scene->collisions_.deallocate(collision_);
		collision_ = nullptr;
	}

	for(Projectile* projectile = scene->projectiles_.getStart(); projectile != scene->projectiles_.getEnd(); ++projectile)
	{
		if(projectile->isValid_ && projectile->isWorking_ && projectile != parent_)
			Resolve(projectile);
	}
	for(Spaceship* ship = scene->ships_.getStart(); ship != scene->ships_.getEnd(); ++ship)
	{
		if(ship->isValid_ && ship->isWorking_ && ship != parent_)
			Resolve(ship);
	}
}

void Collider::Resolve(GameObject* otherObject)
{
	if(!otherObject)
		return;

	Collider* otherCollider = nullptr;
	otherCollider = otherObject->GetCollider();

	if(!otherCollider)
		return;

	if(boundingBox_ == BoundingBoxes::POINT)
	{
		if(otherCollider->boundingBox_ == BoundingBoxes::CIRCLE)
		{
			Direction direction = parent_->GetTransform()->position_ - otherObject->GetTransform()->position_;
			float distance = glm::length(direction);

			if(distance < otherObject->GetTransform()->scale_)
			{
				AllocateCollision(otherObject);
			}
		}
	}
	else if(boundingBox_ == BoundingBoxes::CIRCLE)
	{
		if(otherCollider->boundingBox_ == BoundingBoxes::POINT)
		{
			Direction direction = parent_->GetTransform()->position_ - otherObject->GetTransform()->position_;
			float distance = glm::length(direction);

			if(distance < parent_->GetTransform()->scale_)
			{
				AllocateCollision(otherObject);
			}
		}
	}
}

void Collider::AllocateCollision(GameObject* otherObject)
{
	Scene* scene = GameObject::mainScene_;

	if(!scene)
		nullptr;

	if(collision_)
	{
		scene->collisions_.deallocate(collision_);
		collision_ = nullptr;
	}

	collision_ = scene->collisions_.allocate();
	collision_->Initialize(otherObject);
}

void Collision::Initialize(GameObject* other)
{
	collider_ = other;
}
