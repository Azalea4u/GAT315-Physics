#include "editor.h"
#include "body.h"
#include "render.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data

ncEditorData_t ncEditorData;

Rectangle editorRect;
bool ncEditorActive = true;

Vector2 anchor01 = { 950, 20 };
Texture2D cursorTexture;

bool EditorBoxActive = true;
bool ncEditorIntersect = false;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

	Image image = LoadImage("resources/reticle.png");
	cursorTexture = LoadTextureFromImage(image);
	//UnloadImage(image);
	//HideCursor();

    ncEditorData.anchor01 = (Vector2){ 950, 40 };
    ncEditorData.EditorBoxActive = true;

    // body
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.MassValue = 0.0f;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravityScaleValue = 0.0f;
	ncEditorData.StiffnessValue = 10.0f;
	ncEditorData.RestitutionValue = 0.0f;

    // world
	ncEditorData.GravityValue = 0.0f;
    ncEditorData.GravitationValue = 0.0f;
	ncEditorData.TimeStepValue = 0.0f;

	// simulation
	ncEditorData.ResetSimulation = false;
	ncEditorData.SimulationRunning = false;

	editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 320, 720 };
}

void UpdateEditor(Vector2 position)
{
	// toogle show / hide editor box with key press
	if (IsKeyPressed(KEY_TAB)) ncEditorData.EditorBoxActive = !ncEditorData.EditorBoxActive;

	// check if cursor position is intersecting the editor box
	ncEditorIntersect = EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
    if (ncEditorData.BodyTypeEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive)
    {
        ncEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 600 }, "EDITOR");

        // body
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 56, 272, 232 }, "Body");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 128, 120, 16 }, "Mass", EDITOR_DATA(ncEditorData.MassValue), 0, 5);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 160, 120, 16 }, "Damping", EDITOR_DATA(ncEditorData.DampingValue),0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 192, 120, 16 }, "Gravity Scale", EDITOR_DATA(ncEditorData.GravityScaleValue), 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 224, 120, 16 }, "Stiffness (k)", EDITOR_DATA(ncEditorData.StiffnessValue), 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 256, 120, 16 }, "Resitution", EDITOR_DATA(ncEditorData.RestitutionValue), 0, 1);
        if (GuiDropdownBox((Rectangle) { ncEditorData.anchor01.x + 80, ncEditorData.anchor01.y + 80, 152, 32 }, "DYNAMIC;KINEMATIC;STATIC", & ncEditorData.BodyTypeActive, ncEditorData.BodyTypeEditMode)) ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;

        // world
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 354, 272, 152 }, "World");
        GuiSlider((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 376, 120, 16 }, "Gravity", EDITOR_DATA(ncEditorData.GravityValue), -10, 0);
        GuiSlider((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 408, 120, 16 }, "Gravitation", EDITOR_DATA(ncEditorData.GravitationValue), 0, 20);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 440, 120, 16 }, "Timestep", EDITOR_DATA(ncEditorData.TimeStepValue), 0, 120);

      
    }

    if (GuiButton((Rectangle) { ncEditorData.anchor01.x + 20, ncEditorData.anchor01.y + 536, 120, 40 }, "RESET"))
    {
        ncEditorData.SimulationRunning = false;
        // Reset the simulation state here
		ncEditorData.ResetSimulation = true;
    }

    if (GuiButton((Rectangle) { ncEditorData.anchor01.x + 160, ncEditorData.anchor01.y + 536, 120, 40 }, "SIMULATE"))
    {
        ncEditorData.SimulationRunning = !ncEditorData.SimulationRunning;
    }

    GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);

        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}