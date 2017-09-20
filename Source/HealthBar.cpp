#include "HealthBar.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Ship.hpp"

void HealthBar::Initialize(Ship* ship, Mesh* mesh)
{
	isValid_ = true;
	isWorking_ = true;
	ship_ = ship;
	mesh_ = mesh;
}

void HealthBar::Draw(Matrix& finalMatrix)
{
	if(!ship_)
		return;

	if(!ship_->isValid_)
	{
		isWorking_ = false;
		return;
	}

	//Matrix worldMatrix = ship_->GetTransform()->GetPositionMatrix();
	Matrix worldMatrix = ship_->GetWorldPositionMatrix();

	if(mesh_)
		mesh_->draw(finalMatrix, worldMatrix);
}

void HealthBar::Destroy()
{
	isValid_ = false;
}
