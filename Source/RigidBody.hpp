/*
 * RigidBody.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#ifndef RIGIDBODY_HPP_
#define RIGIDBODY_HPP_

#include <glm.hpp>
#include "Conventions.hpp"

class GameObject;
class Transform;
class Scene;

class RigidBody {
public:
	static float gravityConstant_;

	GameObject* parent_;
	Direction velocity_;
	float angularMomentum_;
	float mass_;
	float drag_;
	float angularDrag_;

	void Initialize(GameObject*, float, float);
	void update(Transform*);
	void updateGravity(GameObject*);
	void Spin(float);
	void Push(Direction);
	void PushForward(float);
	void AddOrbitalVelocity(GameObject*);
	RigidBody();
	RigidBody(GameObject*, float, float);
	virtual ~RigidBody();
	static RigidBody* Allocate(Scene*, GameObject*, float, float);
};

#endif /* RIGIDBODY_HPP_ */
