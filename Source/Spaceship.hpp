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
class HealthBar;

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
	float GetDamage() const {return currentIntegrity_ / maximumIntegrity_;}
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

	HealthBar* healthBar_;

	void updateLogic() override;
	void Initialize(bool, Mesh*, Transform*, RigidBody*) override;
	void OnDraw(Matrix&, Matrix&) override;
	Spaceship();
	virtual ~Spaceship();
	bool IsControlled() override {return true;}
	Weapon* GetWeapon() override {return weapon_;}
	Hull* GetHull() {return &hull_;}
	void Collide(Collision*) override;
};

class Projectile : public GameObject {
public:
	float lifeTime_;

	void Initialize(bool, Mesh*, Transform*, RigidBody*) override;
	void updateLogic() override;
	void Collide(Collision*) override;
};

#endif /* SPACESHIP_HPP_ */
