#include "World.h"
#include "raymath.h"
#include "Integrator.h"
void World::Step(float dt)
{
	
	mousePos = GetMousePosition();
	for (Body& body : bodies)
	{
		body.acceleration = { 0,0 };
		//body.acceleration /=2;
	}
	if (IsKeyDown(KEY_E)) {
		for (Body& body : bodies) {
			body.acceleration = { 100,0 };
		}

	}
	if (IsKeyPressed(KEY_T)) {
		for (Body& body : bodies) {
			body.acceleration = {1/ body.acceleration.x, 1/ body.acceleration.y };
		}

	}
	

	for (Body& body : bodies)
	{
		body.Step(dt);

	}
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		for (Body& body : bodies) {
			Vector2 direction;
			if (IsKeyDown(KEY_TAB)) {
				//push
				direction = body.position - mousePos;
			}
			else {
				//pull
				direction = mousePos - body.position;
			}
			if (Vector2Length(direction) <= 100.0f)
			{
				Vector2 force = Vector2Normalize(direction) * 10000.0f;
				body.AddForce(force);
			}


		}
		
	}
	for (Body& body : bodies)
	{
		SemiImplicitEulerIntegrator(body, dt);

	}
	

	if (IsKeyDown(KEY_Q)) {
		for (Body& body : bodies) {
			body.position = { 0,0 };
		}

	}
	if (IsKeyDown(KEY_W)) {
		for (Body& body : bodies) {
			body.velocity = { 0,0 };
		}

	}
	
	if (IsKeyPressed(KEY_R)) {
		//cycle negative, none, and positive gravity

		if(gravity.y > 0)
			gravity.y = 0;
		else if (gravity.y == 0)
			gravity.y = -9.81f;
		else
			gravity.y = 9.81f;

	}

}

void World::Draw()
{
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) 
	{
		DrawCircleLinesV(mousePos, 100, WHITE);

	}
	for (Body& body : bodies)
	{
		body.Draw();
	}


}

void World::AddBody(const Body& body)
{
	bodies.push_back(body);

}