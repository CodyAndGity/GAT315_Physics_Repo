#include "World.h"
#include "raymath.h"
#include "Integrator.h"
#include "Effector.h"
#include "Collision.h"

void World::Step(float dt)
{
	
	mousePos = GetMousePosition();


	for (Body& body : bodies)
	{
		
		body.acceleration = { 0,0 };
		
	}
	for (Body& body : bodies)
	{
		if (body.bodyType != BodyType::Static) {
			body.AddForce(gravity * body.gravityScale * 100.0f, ForceMode::Acceleration);
		}
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
	if (IsKeyDown(KEY_U)) {
		for (Body& body : bodies) {
			Vector2 direction;
			
			direction = body.position - mousePos;
			
			
			if (Vector2Length(direction) <= 100.0f)
			{
				float length = Vector2Length(body.velocity);
				body.velocity = Vector2Normalize(direction) * length;

				
			}
		}

	}

	
	//fore effector
	for(auto& effector:effectors)
	{
		effector->Apply(bodies);
	}
	
	for (Body& body : bodies)
	{
		if (body.bodyType == BodyType::Dynamic) {
			SemiImplicitEulerIntegrator(body, dt);
		}

	}

	//handle screen bounds
	HandleCollisions();

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
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_U)) 
	{
		DrawCircleLinesV(mousePos, 100, WHITE);

	}
	for (Body& body : bodies)
	{
		body.Draw();
	}
	for (auto& effector : effectors)
	{
		effector->Draw();
	}


}

void World::AddBody(const Body& body)
{
	bodies.push_back(body);

}

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
}

void World::HandleCollisions()
{
	contacts.clear();
	
	Contact::CreateContacts(bodies, contacts);
	Contact::SeparateContacts(contacts);
	Contact::ResolveContacts(contacts);
	for (Body& body : bodies)
	{
		//screen collisions
		if ((body.position.x + body.radius) > GetScreenWidth())
		{
			body.position.x = GetScreenWidth() - body.radius;
			body.velocity.x *= -body.restitution;
		}
		if ((body.position.x - body.radius) < 0)
		{
			body.position.x = body.radius;
			body.velocity.x *= -body.restitution;
		}

		if ((body.position.y + body.radius) > GetScreenHeight())
		{
			body.position.y = GetScreenHeight() - body.radius;
			body.velocity.y *= -body.restitution;
		}

		if ((body.position.y - body.radius) < 0)
		{
			body.position.y = body.radius;
			body.velocity.y *= -body.restitution;
		}

	}

}
