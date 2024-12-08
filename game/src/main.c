#include "body.h"
#include "contact.h"
#include "collision.h"
#include "intergrator.h"
#include "editor.h"
#include "force.h"
#include "mathf.h"
#include "render.h"
#include "spring.h"
#include "World.h"

#include "raylib.h"
#include "raymath.h"

#include <assert.h>
#include <stdlib.h>

int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;
	ncContact_t* contacts = NULL;

	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	// initialize world
	ncGravity = (Vector2){ 0, -1 };
	float fixedTimeStep = 1.0f / 50.0f;
	float timeAccumulator = 0.0f;

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();
		ncGravity = (Vector2){ 0, ncEditorData.GravityValue };

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10.0f);

		UpdateEditor(position);
		fixedTimeStep = 1.0f / ncEditorData.TimeStepValue;

		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines((int)screen.x, (int)screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
		}

		// if cursor is intersecting editor, don't create, connect bodies
		if (!ncEditorIntersect)
		{
			// create body
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)))
			{
				float angle = GetRandomFloatValue(0, 360);
				for (int i = 0; i < 1; i++)
				{
					ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassValue, ncEditorData.BodyTypeActive);

					body->damping = ncEditorData.DampingValue;
					body->gravityScale = ncEditorData.GravityScaleValue;
					body->color = WHITE; // ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
					body->restitution = 0.8f;

					AddBody(body);
				}
			}

			if (IsKeyDown(KEY_LEFT_ALT))
			{
				if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
				if (connectBody)
				{
					Vector2 world = ConvertScreenToWorld(position);
					if (connectBody->type == BT_STATIC || connectBody->type == BT_KINEMATIC)
					{
						connectBody->position = world;
					}
					else
					{
						ApplySpringForcePosition(world, connectBody, 0, 20, 5);
					}
				}
			}
			if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
			{
				selectedBody = NULL;
				connectBody = NULL;
			}
		}

		// connect springs
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
				AddSpring(spring);
			}
		}

		// check if simulation is running
		if (ncEditorData.SimulationRunning)
		{
			timeAccumulator += dt;

			while (timeAccumulator >= fixedTimeStep)
			{
				timeAccumulator -= fixedTimeStep; // Subtract fixed timestep from accumulator

				// Apply forces
				ApplyGravitation(ncBodies, ncEditorData.GravitationValue);
				ApplySpringForce(ncSprings);

				// Update bodies
				for (ncBody* body = ncBodies; body; body = body->next)
				{
					Step(body, fixedTimeStep); // Pass fixedTimestep as the timestep
				}

				// Collision
				DestroyAllContacts(&contacts);
				CreateContacts(ncBodies, &contacts);
				SeparateContacts(contacts);
				ResolveContacts(contacts);
			}
		}

		// reset simulation
		if (ncEditorData.ResetSimulation)
		{
			DestroyAllContacts(&contacts);
			DestoryAllBodies();
			ncEditorData.ResetSimulation = false;
		}

		// render
		BeginDrawing();
		ClearBackground(BLACK);

		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		// draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}
		// draw bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
		}
		// draw contacts
		for (ncContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}

		DrawEditor(position);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}