#include "Collision.h"

void  Contact::CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			if (bodyA.bodyType != BodyType::Dynamic && bodyB.bodyType != BodyType::Dynamic) continue;

			if (Intersects(bodyA, bodyB))
			{
				Contact contact;
				contact.bodyA = &bodyA;
				contact.bodyB = &bodyB;

				Vector2 direction = bodyA.position - bodyB.position;
				float distance = Vector2Length(direction);

					float radius = bodyA.radius + bodyB.radius;
					contact.depth = radius - distance;
					contact.normal = Vector2Normalize(direction);

					contacts.push_back(contact);
			}
		}
	}
}

void Contact::SeparateContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		Vector2 separation = contact.normal * (contact.depth/totalInverseMass);
		contact.bodyA->position = contact.bodyA->position + (separation * contact.bodyA->inverseMass);
		contact.bodyB->position = contact.bodyB->position - (separation * contact.bodyB->inverseMass);
	}
}

bool Contact::Intersects(const Body& bodyA, const Body& bodyB)
{
	float distance = Vector2Length(bodyA.position - bodyB.position);
		float radius = bodyA.radius + bodyB.radius;

	return (distance <=  radius);
}
