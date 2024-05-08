#include "body.h"
#include "intergrator.h"
#include "editor.h"
#include "force.h"
#include "mathf.h"
#include "render.h"
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
	InitEditor();

	// initialize world
	ncGravity = (Vector2){ 0, -1 };

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10.0f);

		UpdateEditor(position);

		if (IsMouseButtonDown(0))
		{
			for (int i = 0; i < 1; i++)
			{
				ncBody* body = CreateBody();
				body->position = ConvertScreenToWorld(position);
				body->mass = GetRandomFloatValue(ncEditorData.MassMinValue, ncEditorData.MassMaxValue);
				body->inverseMass = 1 / body->mass;
				body->type = BT_DYNAMIC;
				body->damping = 0; // 2.5f;
				body->gravityScale = 0;
				body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);

				//Vector2 force = Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360)), GetRandomFloatValue(100, 200));
				//ApplyForce(body, force, FM_VELOCITY); 
			}
		}


		//apply force
		ApplyGravitation(ncBodies, ncEditorData.GravitationValue);

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
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass), body->color);
		}

		DrawEditor();

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