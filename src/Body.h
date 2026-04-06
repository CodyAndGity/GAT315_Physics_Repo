#pragma once
#include "raylib.h"
struct Body {
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	float radius;
	float restitution;
	float gravityScale = 1.0f;
	float damping = 0.0f;
	Color color;

	

	void Step(float dt);
	void Draw();
	void AddForce(Vector2 force);
};

