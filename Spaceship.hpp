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

class Weapon {
public:
	float lastFire_;
	float fireSpeed_;

	void initialize();
	void update();
	bool canFire();
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

class Hull {

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

	void updateLogic() override;
	void initialize(bool, Mesh*, glm::vec3, glm::vec3, float, bool, bool) override;
	Spaceship();
	virtual ~Spaceship();
};

class Projectile : public GameObject {
public:
	float lifeTime_;

	void initialize(bool, Mesh*, glm::vec3, glm::vec3, float, bool, bool) override;
	void updateLogic() override;
};

#endif /* SPACESHIP_HPP_ */
