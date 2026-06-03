#include "Spring.h"

#include "Body.h"
void Spring::Apply(float multiplier)
{
	Vector2 force = GetSpringForce(bodyA->position, bodyB->position, restLength, stiffness * multiplier);
	//damping
	//direction vector from body A to body B (spring axis(
	Vector2 direction = bodyB->position - bodyA->position;
	Vector2 normalizedDirection = Vector2Normalize(direction);
	//relative velocity of body B to body A
	Vector2 relativeVelocity = bodyB->velocity - bodyA->velocity;
	
	//higher damping when moving along spring axis, less when perpendicular to it
	float dampingFactor = Vector2DotProduct(relativeVelocity, normalizedDirection);

	//damping force opposes motion along the spring axis
	Vector2 dampingForce = normalizedDirection * dampingFactor*damping;
	force -= dampingForce;

	bodyA->AddForce(force * -1);
	bodyB->AddForce(force);
}
void Spring::Draw()
{
	DrawLineV(bodyA->position, bodyB->position, WHITE);
}
//returns the force that pulls B to A
Vector2 Spring::GetSpringForce(Vector2 positionA, Vector2 positionB, float restLength, float stiffness)
{
	Vector2 direction = positionB - positionA;
	float length = Vector2Length(direction);
	float displacement = length - restLength;
	// Hooke's law: F = -kx
	float magnitude = -displacement * stiffness;

	return Vector2Normalize(direction)*magnitude;
}


