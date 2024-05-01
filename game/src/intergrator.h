#pragma once
#include "body.h"

void ExplicitEuler(ncBody* body, float timestep)
{
	// p' = p + (v * dt)
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));

	// v' = v + (a * dt)
	body->velocity = Vector2Add(body->velocity, Vector2Scale(Vector2Scale(body->force, 1 / body->mass), timestep));
}