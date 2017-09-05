#include "HealthBar.hpp"
#include "Spaceship.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"

void HealthBar::Initialize(Spaceship* ship, Mesh* mesh)
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

	Matrix worldMatrix = ship_->GetTransform()->GetPositionMatrix();

	if(mesh_)
		mesh_->draw(finalMatrix, worldMatrix);
}

void HealthBar::Destroy()
{
	isValid_ = false;
}