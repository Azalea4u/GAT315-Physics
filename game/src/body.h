#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum ncBodyType
{
	STATIC,
	DYNAMIC
} ncBodyType;

typedef struct ncBody
{
	ncBodyType body;
	// force -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 force;

	float mass;
	float inverseMass; // 1 / mass (static = 0)

	struct ncBody* next;
	struct ncBody* prev;
} ncBody;

inline void ApplyForce(ncBody* body, Vector2 force)
{
	body->force = Vector2Add(body->force, force);
}

inline void ClearForce(ncBody* body)
{
	body->force = Vector2Zero();
}