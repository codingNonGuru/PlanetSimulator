/*
 * RigidBody.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#ifndef RIGIDBODY_HPP_
#define RIGIDBODY_HPP_

#include <glm.hpp>

class GameObject;
class Transform;
typedef glm::vec3 Direction;

class RigidBody {
public:
	GameObject* parent_;
	glm::vec3 velocity_;
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
};

#endif /* RIGIDBODY_HPP_ */
