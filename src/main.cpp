

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include <string>

#include "World.h"
#include "Body.h"
#include "Integrator.h"
#include "Random.h"
#include "PointEffector.h"
#include "GravitationalEffector.h"

int main()
{
	
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	//SetTargetFPS(60);

	bool clearScreen = true;
	World world;
	//world.AddEffector(new PointEffector({ 600,300 }, 100.0f, -10000.0f));
	//world.AddEffector(new PointEffector({ 300,600 }, 100.0f, 10000.0f));
	world.AddEffector(new GravitationalEffector(10000.0f));
	float timeAccumulator = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;

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
			//body.mass = 1;
			body.mass = body.radius;
			body.gravityScale = 0;
			body.damping = 0.1f;
			world.AddBody(body);
		}


		//update
		timeAccumulator += dt;

		while (timeAccumulator >= fixedTimeStep) {

			world.Step(fixedTimeStep);
			timeAccumulator -= fixedTimeStep;

		}


		if(IsKeyPressed(KEY_C))
		{
			clearScreen = !clearScreen;
		}
	
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		if (clearScreen) {
			ClearBackground(BLACK);
		}

		// draw some text using the default font
		std::string fpsText = "FPS: ";
		fpsText+= std::to_string(GetFPS());
		DrawText(fpsText.c_str(), 100, 100, 20, WHITE);

		

		//draw bodies
		world.Draw();
		


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


