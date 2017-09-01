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

class RigidBody {
public:
	glm::vec3 velocity_;
	float angularMomentum_;
	float mass_;
	float drag_;
	float angularDrag_;

	void initialize(GameObject*, float, bool, bool);
	void update(Transform*);
	void updateGravity(GameObject*);
	void Spin(float);
	void Drag(glm::vec3);
	RigidBody();
	virtual ~RigidBody();
};

#endif /* RIGIDBODY_HPP_ */
