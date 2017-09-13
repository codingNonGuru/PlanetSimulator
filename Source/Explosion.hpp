#pragma once
#include "GameObject.hpp"

class Mesh;
class Transform;
class Controller;

class Explosion : public GameObject
{
public:
	float lifeTime_;

	void Initialize(bool, Mesh*, Transform*, RigidBody*, Controller*) override;
	void updateLogic() override;
};
