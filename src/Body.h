#pragma once
#include "raylib.h"
struct Body {
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	float radius;
	float restitution;
	Color color;

	Body(Vector2 position = { 0,0 }, Vector2 velocity = { 0,0 }, Vector2 acceleration = { 0,0 }, float mass = 1.0f, float radius = 10.0f, float restitution = 0.59f, Color color = WHITE)
	{
		this->position = position;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->mass = mass;
		this->radius = radius;
		this->restitution = restitution;
		this->color = color;
	}

	void Step(float dt);
	void Draw();
	void AddForce(Vector2 force);
};

