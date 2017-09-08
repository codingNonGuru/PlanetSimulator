#include "Collider.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"

void Collider::Initialize(GameObject* parent, BoundingBoxes boundingBox)
{
	parent_ = parent;
	boundingBox_ = boundingBox;
}

void Collider::Update()
{
	Scene* scene = GameObject::mainScene_;

	if(!scene)
		return;

	for(Shell* projectile = scene->shells_.getStart(); projectile != scene->shells_.getEnd(); ++projectile)
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

	bool isMeshThisPointOther = boundingBox_ == BoundingBoxes::MESH && otherCollider->boundingBox_ == BoundingBoxes::POINT;
	bool isMeshOtherPointThis = boundingBox_ == BoundingBoxes::POINT && otherCollider->boundingBox_ == BoundingBoxes::MESH;
	if(isMeshThisPointOther || isMeshOtherPointThis)
	{
		Mesh* mesh = nullptr;
		if(isMeshThisPointOther)
			mesh = parent_->mesh_;
		else
			mesh = otherObject->mesh_;

		if(!mesh)
			return;

		Position pointPosition;
		if(isMeshThisPointOther)
			pointPosition = otherObject->GetTransform()->position_;
		else
			pointPosition = parent_->GetTransform()->position_;

		Transform* meshTransform = nullptr;
		if(isMeshThisPointOther)
			meshTransform = parent_->GetTransform();
		else
			meshTransform = otherObject->GetTransform();

		Position vertices[3];
		Direction directions[3];
		float sine = sin(meshTransform->rotation_.z);
		float cosine = cos(meshTransform->rotation_.z);
		for(int i = 0; i < mesh->elementCount_; i += 3)
		{
			for(int j = i; j < i + 3; ++j)
			{
				auto vertex = mesh->GetPosition(j);
				float x = vertex->x * cosine - vertex->y * sine;
				float y = vertex->x * sine + vertex->y * cosine;
				x *= meshTransform->scale_;
				y *= meshTransform->scale_;
				x += meshTransform->position_.x;
				y += meshTransform->position_.y;
				vertices[j - i] = Position(x, y, 0.0f);
			}
			bool isFirstInside = false;
			bool isSecondInside = false;

			directions[0] = vertices[0] - vertices[1];
			directions[1] = vertices[2] - vertices[1];
			directions[2] = pointPosition - vertices[1];
			{
				Direction first = glm::cross(directions[0], directions[2]);
				Direction second = glm::cross(directions[1], directions[2]);
				if(first.z * second.z < 0.0f)
					isFirstInside = true;
			}

			directions[0] = vertices[0] - vertices[2];
			directions[1] = vertices[1] - vertices[2];
			directions[2] = pointPosition - vertices[2];
			{
				Direction first = glm::cross(directions[0], directions[2]);
				Direction second = glm::cross(directions[1], directions[2]);
				if(first.z * second.z < 0.0f)
					isSecondInside = true;
			}

			if(isFirstInside && isSecondInside)
			{
				AllocateCollision(otherObject);
				break;
			}
		}
	}
}

Collision collision;

void Collider::AllocateCollision(GameObject* otherObject)
{
	collision.Initialize(parent_);

	otherObject->Collide(&collision);
}

void Collision::Initialize(GameObject* other)
{
	collider_ = other;
}
