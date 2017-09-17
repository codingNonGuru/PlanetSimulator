#pragma once

#include "GameObject.hpp"
#include "Memory.hpp"

class Ship;

enum class StructureTypes {OUTPOST, DOCK, COUNT};

class Structure : public GameObject
{
	StructureTypes type_;
	container::DynamicPool<Ship*> ships_;
	int lastLaunch_;
	int launchInterval_;

public:
	void SetType(StructureTypes);
	void AddShip(Ship*);
	void UpdateLogic() override;
};
