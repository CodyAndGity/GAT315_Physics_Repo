#include "Body.h"

#include "raymath.h"



void Body::Draw()
{
	DrawCircleV(this->position, this->radius, this->color);
	DrawCircleLinesV(this->position, this->radius, WHITE);

}

void Body::AddForce(Vector2 force, ForceMode mode)
{

	if(bodyType != BodyType::Dynamic)
		return;

	switch (mode)
	{
	case ForceMode::Force:
		acceleration += force * inverseMass;
		break;
	case ForceMode::Impulse:
		velocity += force * inverseMass;
		break;
	case ForceMode::Acceleration:
		acceleration += force;
		break;
	case ForceMode::VelocityChange:
		velocity += force;
		break;
	default:
		break;
	}

}
