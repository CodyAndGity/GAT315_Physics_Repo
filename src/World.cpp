#include "World.h"
#include "raymath.h"
#include "Integrator.h"
void World::Step(float dt)
{
	
	mousePos = GetMousePosition();
	for (Body& body : bodies)
	{
		body.acceleration = { 0,0 };
	}
	for (Body& body : bodies)
	{
		//AddForce(body, gravity * 100.0f);
		body.AddForce(gravity * 100.0f);
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