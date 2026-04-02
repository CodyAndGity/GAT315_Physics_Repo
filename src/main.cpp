/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
typedef struct Body {
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	float radius;
	float restitution;
	Color color;
};
//return 0-1
float GetRandomFloat() {
	return GetRandomValue(0, 10000) / 10000.0f;
}

void AddForce(Body& body, Vector2 force)
{
	body.acceleration += force * (1 / body.mass);
}

void ExplicitEulerIntegrator(Body& body, float dt)
{
	body.position += body.velocity * dt;
	body.velocity += body.acceleration * dt;
}
void SemiImplicitEulerIntegrator(Body& body, float dt)
{
	body.velocity += body.acceleration * dt;
	body.position += body.velocity * dt;
}

Vector2 gravity = { 0,9.81f };
int main()
{
	std::vector<Body>bodies;
	bodies.reserve(1000);
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();
		Vector2 mousePos = GetMousePosition();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			Body body;
			body.position = mousePos;
			float angle = GetRandomFloat() * 2.0f * PI;
			//get random unit circle vector
			Vector2 direction;
			direction.x = cosf(angle);
			direction.y = sinf(angle);

			body.velocity = direction * (GetRandomFloat() * 300 + 20);
			body.acceleration = { 0,0 };
			body.radius = GetRandomFloat() * 20 + 5;
			//body.restitution = 0.5f +(GetRandomFloat()*0.6f);
			body.restitution = 0.9f;
			Color randomColor = { GetRandomValue(0,255),GetRandomValue(0,255),GetRandomValue(0,255),255 };
			body.color = randomColor;
			body.mass = 1;
			bodies.push_back(body);
		}


		//update
		for (Body& body : bodies)
		{
			body.acceleration = { 0,0 };
		}
		for (Body& body : bodies)
		{
			AddForce(body, gravity * 100.0f);
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_TAB))
		{
			for (Body& body : bodies) {
				Vector2 direction = body.position - mousePos;
				if (Vector2Length(direction) <= 100.0f)
				{
					Vector2 force = Vector2Normalize(direction) * 10000.0f;
					AddForce(body, force);
				}


			}
			DrawCircleLinesV(mousePos, 100, WHITE);
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			for (Body& body : bodies) {
				Vector2 direction = mousePos - body.position;
				if (Vector2Length(direction) <= 100.0f)
				{
					Vector2 force = Vector2Normalize(direction) * 10000.0f;
					AddForce(body, force);
				}


			}
			DrawCircleLinesV(mousePos, 100, WHITE);
		}



		for (Body& body : bodies)
		{
			SemiImplicitEulerIntegrator(body, dt);

		}

		for (Body& body : bodies)
		{




			//screen collsions
			if ((body.position.x + body.radius) > GetScreenWidth())
			{
				body.position.x = GetScreenWidth() - +body.radius;
				body.velocity.x *= -body.restitution;
			}
			if ((body.position.x - body.radius) < 0)
			{
				body.position.x = body.radius;
				body.velocity.x *= -body.restitution;
			}

			if ((body.position.y + body.radius) > GetScreenHeight())
			{
				body.position.y = GetScreenHeight() - +body.radius;
				body.velocity.y *= -body.restitution;
			}
			/*if((body.position.y-body.radius) < 0)
			{
				body.position.y = body.radius;
				body.velocity.y *= -body.restitution;
			}*/
		}

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Hello Raylib", 200, 200, 20, WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		//draw bodies

		for (const Body& body : bodies)
		{
			DrawCircleV(body.position, body.radius, body.color);
		}


		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
