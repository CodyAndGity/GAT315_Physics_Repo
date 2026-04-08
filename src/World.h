#pragma once
#include "raylib.h"
#include "Body.h"
#include "Collision.h"
#include <vector>

class World
{
public:
	World() {
		bodies.reserve(1000);
	}
	Vector2 gravity = { 0,9.81f };
	std::vector<Body>bodies;
	std::vector<class Effector*> effectors;
	std::vector<Contact> contacts;
	Vector2 mousePos = { 0,0 };

	void Step(float dt);
	void Draw();
	void AddBody(const Body& body);
	void AddEffector(class Effector* effector);
	void HandleCollisions();
	
};

