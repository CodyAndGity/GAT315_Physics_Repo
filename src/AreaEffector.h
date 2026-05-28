#pragma once
#include "Effector.h"
class AreaEffector : public Effector
{
public:

	AreaEffector(Vector2 position, float size,float angle, float forceMagnitude)
		: Effector(position, size), angle(angle), forceMagnitude(forceMagnitude)
	{
	}

	// Inherited via Effector
	void Apply(std::vector<Body>& ibodies) override;
	void Draw() override;
private:
	float angle;
	float forceMagnitude;
};