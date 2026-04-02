#include "Body.h"

#include "raymath.h"

void Body::Step(float dt)
{
	//this->AddForce(gravity * 100.0f);
	//screen collsions
	if ((this->position.x + this->radius) > GetScreenWidth())
	{
		this->position.x = GetScreenWidth() - this->radius;
		this->velocity.x *= -this->restitution;
	}
	if ((this->position.x - this->radius) < 0)
	{
		this->position.x = this->radius;
		this->velocity.x *= -this->restitution;
	}

	if ((this->position.y + this->radius) > GetScreenHeight())
	{
		this->position.y = GetScreenHeight() - this->radius;
		this->velocity.y *= -this->restitution;
	}
	
	/*if ((this->position.y - this->radius) < 0)
	{
		this->position.y = this->radius;
		this->velocity.y *= -this->restitution;
	}*/
}

void Body::Draw()
{
	DrawCircleV(this->position, this->radius, this->color);

}

void Body::AddForce(Vector2 force)
{
	this->acceleration += force * (1 / this->mass);

}
