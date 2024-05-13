#include "editor.h"
#include "body.h"
#include "render.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 950, 20 };
Texture2D cursorTexture;

bool EditorBoxActive = true;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

	Image image = LoadImage("resources/reticle.png");
	cursorTexture = LoadTextureFromImage(image);
	UnloadImage(image);
	HideCursor();

    ncEditorData.anchor01 = (Vector2){ 950, 40 };
    ncEditorData.EditorBoxActive = true;
    ncEditorData.DropdownBoxEditMode = false;
    ncEditorData.DropdownBoxActive = 0;
    ncEditorData.MassMinBarValue = 0.0f;
    ncEditorData.MassMaxBarValue = 0.0f;
    ncEditorData.DampingBarValue = 0.0f;
    ncEditorData.GravityScaleBarValue = 0.0f;
    ncEditorData.GravitationBarValue = 0.0f;

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
    if (ncEditorData.DropdownBoxEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive)
    {
        ncEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 600 }, "EDITOR");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 56, 272, 200 }, "Body");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 128, 120, 16 }, "Mass Min", NULL, & ncEditorData.MassMinBarValue, 0, 5);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 160, 120, 16 }, "Mass Max", NULL, & ncEditorData.MassMaxBarValue, 0, 5);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 192, 120, 16 }, "Damping", NULL, & ncEditorData.DampingBarValue, 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 224, 120, 16 }, "Gravity Scale", NULL, & ncEditorData.GravityScaleBarValue, 0, 10);
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 288, 272, 120 }, "World");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 312, 120, 16 }, "Gravitation", NULL, & ncEditorData.GravitationBarValue, 0, 20);
        if (GuiDropdownBox((Rectangle) { ncEditorData.anchor01.x + 80, ncEditorData.anchor01.y + 80, 152, 32 }, "DYNAMIC;KINEMATIC;STATIC", & ncEditorData.DropdownBoxActive, ncEditorData.DropdownBoxEditMode)) ncEditorData.DropdownBoxEditMode = !ncEditorData.DropdownBoxEditMode;
    }

	DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, WHITE);

    GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);

        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
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