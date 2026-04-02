#pragma once
#include "raylib.h"
#include "Body.h"
#include <vector>

class World
{
public:
	Vector2 gravity = { 0,9.81f };
	std::vector<Body>bodies;

	

	void Step(float dt);
	void Draw();
	void AddBody(const Body& body);
};

