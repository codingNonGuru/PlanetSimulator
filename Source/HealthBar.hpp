#pragma once

#include <glm.hpp>

class Spaceship;
class Mesh;
typedef glm::mat4 Matrix;

class HealthBar
{
	bool isValid_;
	bool isWorking_;
	Spaceship* ship_;
	Mesh* mesh_;

public:
	bool IsValid() const {return isValid_;}
	bool IsWorking() const {return isWorking_;}
	void Initialize(Spaceship*, Mesh*);
	void Draw(Matrix&);
	Mesh* GetMesh() {return mesh_;}
	Spaceship* GetShip() {return ship_;}
	void Destroy();
};
