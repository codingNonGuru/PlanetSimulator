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
class Collider;
class Collision;
typedef glm::mat4 Matrix;
typedef glm::vec3 Position;
typedef glm::vec3 Rotation;

class GameObject {
public:
	static Scene* mainScene_;

	GameObject* parent_;
	GameObject* home_;
	char name_[16];
	Mesh* mesh_;
	Transform* transform_;
	RigidBody* rigidBody_;
	Controller* controller_;
	Collider* collider_;
	bool isValid_, isWorking_, isAttached_;

	virtual void Initialize(bool, Mesh*, Transform*, RigidBody*, Controller*);
	virtual void Draw(Matrix&);
	virtual void OnDraw(Matrix&, Matrix&) {}
	virtual void updatePhysics();
	virtual void UpdateCollisions();
	virtual void updateGravity();
	virtual void UpdateLogic() {}
	GameObject();
	virtual ~GameObject();
	RigidBody*& GetRigidBody() { return rigidBody_; }
	Transform*& GetTransform() { return transform_; }
	Controller*& GetController() { return controller_; }
	Collider* GetCollider() {return collider_;}
	virtual Weapon* GetWeapon() {return nullptr;}
	virtual bool IsControlled() {return false;}
	void SetParent(GameObject* parent) {parent_ = parent;}
	void SetHome(GameObject* home) {home_ = home;}
	virtual void Collide(Collision*) {}
	void Destroy();
	Position GetWorldPosition();
	Rotation GetWorldRotation();
};

#endif /* GAMEOBJECT_HPP_ */
