#include "body.h"
#include "intergrator.h"
#include "mathf.h"
#include "World.h"

#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 10000

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	// initialize world
	ncGravity = (Vector2){ 0, 30 };

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
			body->mass = GetRandomFloatValue(1, 10);
			body->inverseMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.5f;
			body->gravityScale = 20.0f;
			ApplyForce(body, (Vector2){ GetRandomFloatValue(-20, 20), GetRandomFloatValue(-20, 20) }, FM_VELOCITY);
		}

		// apply force
		ncBody* body = ncBodies;
		while (body)
		{
			//ApplyForce(body, CreateVector2(0, -100), FM_FORCE);
			body = body->next;
		}

		// update bodies
		body = ncBodies;
		while (body)
		{
			Step(body, dt);
			body = body->next;
		}

		// render
		BeginDrawing();
		ClearBackground(BLACK);
		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 10, YELLOW);

		body = ncBodies;
		while (body)
		{
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, RED);
			body = body->next;
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}