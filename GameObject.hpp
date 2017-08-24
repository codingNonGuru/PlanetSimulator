/*
 * GameObject.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#ifndef GAMEOBJECT_HPP_
#define GAMEOBJECT_HPP_

#include <glm.hpp>

class Mesh;
class Transform;
class RigidBody;
class Controller;
class Scene;
class Weapon;

class GameObject {
public:
	static Scene* mainScene_;

	bool isValid_, isWorking_;
	Mesh* mesh_;
	Transform* transform_;
	RigidBody* rigidBody_;
	Controller* controller_;

	virtual void initialize(bool, Mesh*, glm::vec3, glm::vec3, float, bool, bool);
	virtual void draw(glm::mat4&);
	virtual void updatePhysics();
	virtual void updateGravity();
	virtual void updateLogic() {}
	GameObject();
	virtual ~GameObject();
	RigidBody*& GetRigidBody() { return rigidBody_; }
	Transform*& GetTransform() { return transform_; }
	virtual Weapon* GetWeapon() {return nullptr;}
	virtual bool IsControlled() {return false;}
};

#endif /* GAMEOBJECT_HPP_ */
