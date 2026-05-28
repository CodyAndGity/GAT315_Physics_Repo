#pragma once
#include "Effector.h"
class PointEffector : public Effector
{
public:

	PointEffector(Vector2 position, float size, float forceMagnitude)
		: Effector(position, size), forceMagnitude(forceMagnitude)
	{
	}

	// Inherited via Effector
	void Apply(std::vector<Body>& ibodies) override;
	void Draw() override;
private:

	float forceMagnitude;
};