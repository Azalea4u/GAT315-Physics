#include "force.h"
#include "body.h"
#include "World.h"

void ApplyGravitation(ncBody* bodies, float strength)
{
	for (ncBody* body1 = bodies; body1 < bodyCount; body1->next)
	{
		for (ncBody* body2 = bodies; body2 < bodyCount; body2->next)
		{
			if (body1 == body2) continue;

			Vector2 direction = Vector2Subtract(body2->position, body1->position);
			float distance = Vector2Length(direction);

			// distance <clamp distance to a min of 1
			distance = fmaxf(1.0f, distance);
			// float force = <(mass1 * mass2) / distance^2 * strength>
			float force = (body1->mass * body2->mass) / (distance * distance) * strength;

			// direction = <scale normalized direction vector by force>
			direction = Vector2Normalize(direction);

			ApplyForce(body1, Vector2Scale(direction, force), FM_FORCE);
			ApplyForce(body2, Vector2Scale(direction, -force), FM_FORCE);
		}
	}
}
