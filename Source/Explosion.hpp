#pragma once
#include "GameObject.hpp"

class Mesh;
class Transform;

class Explosion : public GameObject
{
public:
	float lifeTime_;

	void initialize(bool, Mesh*, Transform*, float, bool, bool) override;
	void updateLogic() override;
};
