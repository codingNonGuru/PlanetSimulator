#pragma once
#include "Memory.hpp"

class HealthBar;

class Interface
{
public:
	container::DynamicPool<HealthBar> healthBars_;

	void Initialize();
	void CleanUp();
};
