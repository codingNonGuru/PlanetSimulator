/*
 * Spaceship.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#ifndef SPACESHIP_HPP_
#define SPACESHIP_HPP_

#include <iostream>

#include "GameObject.hpp"

typedef float Temperature;
typedef float Power;

class Projectile;

class Weapon {
public:
	float lastFire_;
	float fireSpeed_;
	Temperature temperature_;
	float coolingRate_;
	Temperature overheatPoint_;
	bool isCooling_;

	void initialize(float, Temperature, float);
	void update();
	bool CanFire();
	void Fire();
	float GetHeatFactor();
	void Cool();
};

class Sensor {
	GameObject* closestObject_;
	float distanceLimit_;
public:
	Sensor() {
		closestObject_ = nullptr;
		distanceLimit_ = 50.0f;
	}
	void SetObject(GameObject* object) {closestObject_ = object;}
	GameObject* GetObject() {return closestObject_;}
	float GetLimit() {return distanceLimit_;}
	void SetLimit(float distanceLimit) {distanceLimit_ = distanceLimit;}
};

class Thruster {
public:
	float fuel_;

};

class Hull {
public:
	float maximumIntegrity_;
	float currentIntegrity_;

	void Initialize(float integrity) {maximumIntegrity_ = integrity; currentIntegrity_ = integrity;}
	void Damage(Projectile*);
};

class Cargo {
	float mineralOre_;
public:
	Cargo() {mineralOre_ = 0.0f;}
	void AddOre(float ore) {mineralOre_ += ore;}
	float GetOre() const {return mineralOre_;}
};

class Spaceship : public GameObject {
public:
	Sensor sensor_;
	Weapon *weapon_;
	Cargo cargo_;
	Hull hull_;

	void updateLogic() override;
	void initialize(bool, Mesh*, Transform*, float, bool, bool) override;
	Spaceship();
	virtual ~Spaceship();
	bool IsControlled() override {return true;}
	Weapon* GetWeapon() override {return weapon_;}
};

class Projectile : public GameObject {
public:
	float lifeTime_;

	void initialize(bool, Mesh*, Transform*, float, bool, bool) override;
	void updateLogic() override;
};

#endif /* SPACESHIP_HPP_ */
