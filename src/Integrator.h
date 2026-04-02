#pragma once
#include "Body.h"
#include "raymath.h"
inline void ExplicitEulerIntegrator(Body& body, float dt)
{
	body.position += body.velocity * dt;
	body.velocity += body.acceleration * dt;
}
inline void SemiImplicitEulerIntegrator(Body& body, float dt)
{
	body.velocity += body.acceleration * dt;
	body.position += body.velocity * dt;
}