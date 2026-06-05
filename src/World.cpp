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
		if (body.bodyType != BodyType::Static) {
			body.AddForce(gravity * body.gravityScale, ForceMode::Acceleration);
		}
	}
	/*if (IsKeyDown(KEY_E)) {
		for (Body& body : bodies) {
			body.acceleration = { 100,0 };
		}

	}
	if (IsKeyPressed(KEY_T)) {
		for (Body& body : bodies) {
			body.acceleration = { 1 / body.acceleration.x, 1 / body.acceleration.y };
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

	}*/


	//fore effector
	for (auto& effector : effectors)
	{
		effector->Apply(bodies);
	}

	for (auto& spring : springs)
	{
		spring->Apply(springMultiplier);
	}

	for (Body& body : bodies)
	{
		if (body.bodyType == BodyType::Dynamic) {
			SemiImplicitEulerIntegrator(body, dt);
		}

	}

	//handle screen bounds
	for (size_t i = 0; i < 4; i++)
	{
		HandleCollisions();
	}


	if (IsKeyDown(KEY_Q)) {
		for (Body& body : bodies) {
			if (body.bodyType != BodyType::Static) {
				body.position = { 0,0 };
			}
		}

	}
	if (IsKeyDown(KEY_W)) {
		for (Body& body : bodies) {
			body.velocity = { 0,0 };
		}

	}

	//if (IsKeyPressed(KEY_R)) {
	//	//cycle negative, none, and positive gravity

	//	if (gravity.y > 0)
	//		gravity.y = 0;
	//	else if (gravity.y == 0)
	//		gravity.y = -9.81f;
	//	else
	//		gravity.y = 9.81f;

	//}

	for (Body& body : bodies)
	{

		body.acceleration = { 0,0 };

	}

}

void World::Draw()
{
	//vertical lines
	DrawLineV(Vector2{ 0, boundsMin.x }, Vector2{ 0, boundsMax.y }, WHITE);
	for (float x = 1; x < (boundsMax.x - boundsMin.x) * .5f; x += 1)
	{
		DrawLineV(Vector2{ x, boundsMin.x }, Vector2{ x, boundsMax.y }, GRAY);
		DrawLineV(Vector2{ -x, boundsMin.x }, Vector2{ -x, boundsMax.y }, GRAY);
	}
	//horizontal lines
	DrawLineV(Vector2{ boundsMin.x,0 }, Vector2{ boundsMax.x,0 }, WHITE);
	for (float y = 1; y < (boundsMax.y - boundsMin.y) * .5f; y += 1)
	{
		DrawLineV(Vector2{ boundsMin.x, y }, Vector2{ boundsMax.x, y }, GRAY);
		DrawLineV(Vector2{ boundsMin.x, -y }, Vector2{ boundsMax.x, -y }, GRAY);
	}


	for (auto& effector : effectors)
	{
		effector->Draw();
	}
	/*if (IsKeyDown(KEY_U))
	{
		DrawCircleLinesV(mousePos, 100, WHITE);

	}*/
	for (Body& body : bodies)
	{
		body.Draw();
	}
	for (Spring* spring : springs)
	{
		spring->Draw();
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

void World::AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness, float damping)
{
	Spring* spring = new Spring(&bodyA, &bodyB, restLength, stiffness, damping);
	springs.push_back(spring);
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
		if ((body.position.x + body.radius) > boundsMax.x)
		{
			body.position.x = boundsMax.x - body.radius;
			body.velocity.x *= -body.restitution;
		}
		if ((body.position.x - body.radius) < boundsMin.x)
		{
			body.position.x = boundsMin.x + body.radius;
			body.velocity.x *= -body.restitution;
		}

		if ((body.position.y + body.radius) > boundsMax.y)
		{
			body.position.y = boundsMax.y - body.radius;
			body.velocity.y *= -body.restitution;
		}

		if ((body.position.y - body.radius) < boundsMin.y)
		{
			body.position.y = boundsMin.y + body.radius;
			body.velocity.y *= -body.restitution;
		}

	}

}

Body* World::GetBodyIntersect(Vector2 position)
{
	for (Body& body : bodies)
	{
		if (CheckCollisionPointCircle(position, body.position, body.radius))
		{
			return &body;
		}
	}
	return nullptr;
}
