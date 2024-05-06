#include "body.h"
#include "intergrator.h"
#include "force.h"
#include "mathf.h"
#include "World.h"

#include "raylib.h"
#include "raymath.h"

#include <assert.h>
#include <stdlib.h>

#define MAX_BODIES 10000

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	// initialize world
	ncGravity = (Vector2){ 0, 0 };

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps =(float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			ncBody* body = CreateBody();
			body->position = position;
			body->prevPosition = position;
			body->mass = GetRandomFloatValue(1, 10);
			body->inverseMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.5f;
			body->gravityScale = 20.0f;
			body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);

			//ApplyForce(body, (Vector2){ GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);

			Vector2 force = Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360)), GetRandomFloatValue(100, 200));
			ApplyForce(body, force, FM_VELOCITY);
		}

		//ncBody* body = ncBodies;

		//apply force
		ApplyGravitation(ncBodies, 30);

		// update bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			Step(body, dt);
		}

		/*
		// update bodies
		body = ncBodies;
		while (body)
		{
			Step(body, dt);
			body = body->next;
		}
		*/

		// render
		BeginDrawing();
		ClearBackground(BLACK);
		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 10, YELLOW);

		// draw bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);
		}

		/*
		body = ncBodies;
		while (body)
		{
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);
			body = body->next;
		}
		*/

		EndDrawing();
	}

	CloseWindow();

	return 0;
}