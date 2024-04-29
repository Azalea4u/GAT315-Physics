#include "Body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "World.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 1000

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");

	while (!WindowShouldClose())
	{
		SetTargetFPS(60);

		/*
		Body* bodies = (Body*)malloc(sizeof(MAX_BODIES));
		assert(bodies);

		int bodyCount = 0;
		*/

		// game loop
		while (!WindowShouldClose())
		{
			// update
			float dt = GetFrameTime();
			float fps =(float)GetFPS();

			Vector2 position = GetMousePosition();
			if (IsMouseButtonDown(0))
			{
				/*
				bodies[bodyCount].position = position;
				bodies[bodyCount].velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
				bodyCount++;
				*/

				Body* newBody = CreateBody();
				newBody->position = position;
				newBody->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
			}

			// render
			BeginDrawing();
			ClearBackground(BLACK);

			DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
			DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

			DrawCircle((int)position.x, (int)position.y, 10, YELLOW);
			
			Body* currentBody = bodies;
			while (currentBody)
			{
				currentBody->position = Vector2Add(currentBody->position, Vector2Scale(currentBody->velocity, dt));
				DrawCircle((int)currentBody->position.x, (int)currentBody->position.y, 5, RED);

				currentBody = currentBody->next;
			}

			EndDrawing();
		}
	}

	CloseWindow();

	return 0;
}