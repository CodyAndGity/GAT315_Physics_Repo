#pragma once
#include "raylib.h"
 enum class BodyType {
	Dynamic,
	Kinematic,
	Static
};
 enum class ForceMode {
	Force,
	Impulse,
	Acceleration,
	VelocityChange
};

struct Body {

	BodyType bodyType{ BodyType::Dynamic };

	Vector2 position{ 0,0 };
	Vector2 velocity{ 0,0 };
	Vector2 acceleration{ 0,0 };

	float mass{ 1 };
	float inverseMass = 1.0f;//1/mass;
	float radius{ 1 };
	float restitution{ 1.0f };
	float gravityScale{ 1.0f };
	float damping{ 0.0f };
	Color color;

	

	
	void Draw();
	//void AddForce(Vector2 force);
	void AddForce(Vector2 force, ForceMode mode = ForceMode::Force);
};

