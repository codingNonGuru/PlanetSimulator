#include "Structure.hpp"
#include "Ship.hpp"

void Structure::SetType(StructureTypes type)
{
	type_ = type;

	if(type_ == StructureTypes::DOCK)
	{

	}
	else if(type_ == StructureTypes::OUTPOST)
	{
		ships_.initialize(16);
		for(auto ship = ships_.getStart(); ship != ships_.getEnd(); ++ship)
		{
			*ship = nullptr;
		}
		lastLaunch_ = 0;
		launchInterval_ = 200;
	}
}

void Structure::AddShip(Ship* ship)
{
	*ships_.allocate() = ship;
}

void Structure::UpdateLogic()
{
	lastLaunch_++;
	if(lastLaunch_ < launchInterval_)
		return;

	for(auto shipPointer = ships_.getStart(); shipPointer != ships_.getEnd(); ++shipPointer)
	{
		auto ship = *shipPointer;
		if(ship && ship->IsIdle())
		{
			ship->AssignMission(Missions::MINE);
			lastLaunch_ = 0;
			break;
		}
	}
}
