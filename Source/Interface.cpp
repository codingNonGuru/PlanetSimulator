#include "Interface.hpp"
#include "HealthBar.hpp"

void Interface::Initialize()
{
	healthBars_.initialize(32);
}

void Interface::CleanUp()
{
	for(auto healthBar = healthBars_.getStart(); healthBar != healthBars_.getEnd(); ++healthBar)
	{
		if(healthBar->IsValid() && !healthBar->IsWorking())
		{
			healthBar->Destroy();
			healthBars_.deallocate(healthBar);
		}
	}
}
