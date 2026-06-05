

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
#include "AreaEffector.h"
#include "DragEffector.h"
#include "GravitationalEffector.h"
#include "Spring.h"
#include "world_camera.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning(push)
#pragma warning (disable : 4576)
#include "gui_physics.h"
#pragma warning(pop)


GuiPhysicsState state;
void AddBody(Vector2& mousePos, World& world, WorldCamera& camera);

void AddEffector(Vector2& mousePos, World& world, WorldCamera& camera);
int main()
{

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Get GUI state 
	state = InitGuiPhysics();

	GuiLoadStyle("raygui/styles/dark/style_dark.rgs");
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	//SetTargetFPS(60);

	bool clearScreen = true;
	World world;
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 5);
	// set min (left-bottom) boundary(0, screen height) and max (right, top) boundary(screen width, 0)
	world.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	
	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;
	
	float timeAccumulator = 0.0f;
	bool simulate = true;
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float fixedTimeStep = 1.0f / state.FPSValue;
		float dt = GetFrameTime();
		dt = fminf(dt, 0.1f);

		Vector2 mousePos = GetMousePosition();
		if (IsKeyPressed(KEY_SPACE)) {
			state.SimulateActive = !state.SimulateActive;
		}
		if (IsKeyPressed(KEY_TAB)) {
			state.PhysicsPanelActive = !state.PhysicsPanelActive;
		}



		world.gravity = { 0,state.GravityValue };
		world.springMultiplier =state.SpringMultiplierValue ;
		bool isMouseOverGui = state.PhysicsPanelActive && CheckCollisionPointRec(mousePos, Rectangle{ state.anchor02.x + 0, state.anchor02.y + 0, 304, 664 });
		if (!isMouseOverGui) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
				(IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
				{
					AddEffector(mousePos, world, world_camera);
				}
				else
				{
					AddBody(mousePos, world,world_camera);
				}
			} 
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
				//world camrea.Screen to world (mouse pos_)
				selectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(mousePos));
			}



			if (selectedBody) {
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
					if (IsKeyDown(KEY_LEFT_CONTROL)) {
						Vector2 position = world_camera.ScreenToWorld(mousePos);
						Vector2 force = Spring::GetSpringForce(position, selectedBody->position, 1.0f, 1.0f);
						selectedBody->AddForce(force, ForceMode::VelocityChange);

					}
					else {
						connectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(mousePos));

					}
					//DrawLineV(selectedBody->position, world_camera.ScreenToWorld(mousePos), GREEN);

				}
				else {
					if(selectedBody && connectedBody && selectedBody != connectedBody)
					{
						float distance = (state.SpringAutoLengthChecked)? Vector2Distance(selectedBody->position, connectedBody->position):state.SpringLengthValue;
						world.AddSpring(*selectedBody, *connectedBody, distance, state.SpringStiffnessValue,state.SpringDampingValue);
					}
					selectedBody = nullptr;
					connectedBody = nullptr;
				}
			}

		}

		if (state.SimulateActive) {

			//update
			timeAccumulator += dt;

			while (timeAccumulator >= fixedTimeStep) {

				world.Step(fixedTimeStep);
				timeAccumulator -= fixedTimeStep;

			}
		}


		if (IsKeyPressed(KEY_C))
		{
			clearScreen = !clearScreen;
		}

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		if (clearScreen) {
			ClearBackground(BLACK);
			//ClearBackground(Color{ 0, 0, 255, 0 });
		}
		
		
		

		// draw some text using the default font
		std::string fpsText = "FPS: ";
		
		fpsText += std::to_string(GetFPS());
		DrawText(fpsText.c_str(), GetScreenWidth() - 100, 100, 20, WHITE);
		
		//draw bodies
		world_camera.Begin(); // set world camera
		world.Draw(); // draw using world camera transform
		
		
		
		DrawCircleLinesV(world_camera.ScreenToWorld(mousePos), state.BodySizeValue, BLUE);
		if (selectedBody) {
			if (IsKeyDown(KEY_LEFT_CONTROL)) {
				DrawLineV(selectedBody->position, world_camera.ScreenToWorld(mousePos), GREEN);

			}
			DrawCircleLinesV(selectedBody->position, selectedBody->radius*1.05f, RED);
		}
		if (connectedBody) {
			DrawCircleLinesV(connectedBody->position, connectedBody->radius*1.05f, GREEN);
		}
		
		world_camera.End(); // remove world camera
		


		

		GuiPhysics(&state);

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



void AddBody(Vector2& mousePos, World& world, WorldCamera& camera) {
	Body body;

	body.bodyType = (BodyType)state.BodyTypeActive;

	body.position = camera.ScreenToWorld(mousePos);
	float angle = GetRandomFloat() * 2.0f * PI;
	//get random unit circle vector
	Vector2 direction;
	direction.x = cosf(angle);
	direction.y = sinf(angle);

	body.AddForce(direction * state.BodyVelocityValue, ForceMode::VelocityChange);




	body.acceleration = { 0,0 };
	body.radius = state.BodySizeValue;
	//body.restitution = 0.5f +(GetRandomFloat()*0.6f);
	body.restitution = state.BodyRestitutionValue;
	Color randomColor = ColorFromHSV(GetRandomValue(0, 360), 1.0f, 1.0f);//{ GetRandomValue(0,255),GetRandomValue(0,255),GetRandomValue(0,255),255 };
	body.color = randomColor;
	//body.mass = 1;
	body.mass = body.radius * state.BodyMassValue;
	body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;

	body.gravityScale = state.BodyGravityValue;
	body.damping = state.BodyDampingValue;
	world.AddBody(body);
}

void AddEffector(Vector2& mousePos, World& world, WorldCamera& camera) {
	mousePos= camera.ScreenToWorld(mousePos);
	Effector* effector = nullptr;
	switch (state.EffectorTypeActive)
	{
	case 0:
		effector = (new GravitationalEffector(mousePos, state.EffectorSizeValue, state.EffectorForceValue));
		break;
	case 1:
		effector = (new PointEffector(mousePos, state.EffectorSizeValue, state.EffectorForceValue));
		break;
	case 2:
		effector = (new AreaEffector(mousePos, state.EffectorSizeValue, state.EffectorAngleValue, state.EffectorForceValue));
		break;
	case 3:
		effector = (new DragEffector(mousePos, state.EffectorSizeValue, (state.EffectorForceValue >1500? 1500 : state.EffectorForceValue)));
		break;
	default:
		break;
	}
	if (effector) world.AddEffector(effector);
}
