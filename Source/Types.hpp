#pragma once

enum class ShipTypes
{
	SCOUT, CORVETTE, FRIGATE, CRUISER, BARGE, COUNT
};

enum class Actions
{
	THRUST, RETURN, BREAK, STEER_RIGHT, STEER_LEFT, SHOOT, COOL, MINE, UNLOAD, DETACH, COUNT
};

enum class Missions
{
	MINE, DELIVER, DEFEND, ESCORT, COUNT
};

enum class ParticleSystems
{
	EXPLOSION,
	SHELL_HIT,
	COUNT
};
