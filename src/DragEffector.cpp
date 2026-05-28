#include "DragEffector.h"

//force=-velocity*drag;

void DragEffector::Apply(std::vector<Body>& ibodies)
{
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);
	for (auto& body : bodies)
	{
		Vector2 force = (body->velocity * -1) * drag;
		body->AddForce(force);
	}
}

void DragEffector::Draw()
{
	Effector::Draw();
	DrawCircleV(position, size, Fade(YELLOW, 0.2f));
}