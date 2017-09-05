#pragma once
#include "GameObject.hpp"

class Mesh;
class Transform;

class Explosion : public GameObject
{
public:
	float lifeTime_;

	void Initialize(bool, Mesh*, Transform*, RigidBody*) override;
	void updateLogic() override;
};
