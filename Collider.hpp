#pragma once

enum class BoundingBoxes {POINT, CIRCLE, MESH};

class GameObject;

class Collision
{
public:
	GameObject* collider_;

	void Initialize(GameObject*);
};

class Collider
{
public:
	GameObject* parent_;
	BoundingBoxes boundingBox_;
	Collision* collision_;

	void Initialize(GameObject*, BoundingBoxes);
	void Update();
	void Resolve(GameObject*);
	void AllocateCollision(GameObject*);
};
