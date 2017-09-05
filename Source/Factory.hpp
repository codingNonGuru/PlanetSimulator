#pragma once

#include "Types.hpp"

class Spaceship;
class Scene;
class Interface;
class Transform;

class ShipFactory
{
	static Scene* scene_;
	static Interface* interface_;
public:
	static Spaceship* Produce(bool, ShipTypes, Transform*);
	static void SetScene(Scene* scene) {scene_ = scene;}
	static void SetInterface(Interface* interface) {interface_ = interface;}
};
