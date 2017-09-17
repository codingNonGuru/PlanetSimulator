#pragma once

#include <glm.hpp>

class Ship;
class Mesh;
typedef glm::mat4 Matrix;

class HealthBar
{
	bool isValid_;
	bool isWorking_;
	Ship* ship_;
	Mesh* mesh_;

public:
	bool IsValid() const {return isValid_;}
	bool IsWorking() const {return isWorking_;}
	void Initialize(Ship*, Mesh*);
	void Draw(Matrix&);
	Mesh* GetMesh() {return mesh_;}
	Ship* GetShip() {return ship_;}
	void Destroy();
};
