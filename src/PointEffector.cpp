#include "PointEffector.h"
#include "raymath.h"
void PointEffector::Apply(std::vector<Body>& ibodies)
{
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);
	for (auto& body : bodies) 
	{
		Vector2 direction = position- body->position;
		//DrawLine(position.x, position.y, body->position.x, body->position.y, GREEN);//Fade(RED, 0.2f));
		Vector2 force = Vector2Normalize(direction) * forceMagnitude;
		body->AddForce(force);
	}
}
void PointEffector::Draw() {
	Effector::Draw();
	DrawCircleV(position, size, Fade(RED, 0.2f));

}
