#include "editor.h"
#include "body.h"
#include "render.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

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
	UnloadImage(image);
	HideCursor();

    ncEditorData.anchor01 = (Vector2){ 950, 40 };
    ncEditorData.EditorBoxActive = true;

    // body
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.MassValue = 0.0f;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravityScaleValue = 0.0f;
	ncEditorData.StiffnessValue = 10.0f;

    // world
	ncEditorData.GravityValue = 0.0f;
    ncEditorData.GravitationValue = 0.0f;

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
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 56, 272, 200 }, "Body");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 288, 272, 120 }, "World");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 128, 120, 16 }, "Mass", TextFormat("%0.2f", ncEditorData.MassValue), & ncEditorData.MassValue, 0, 5);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 160, 120, 16 }, "Damping", TextFormat("%0.2f", ncEditorData.DampingValue), & ncEditorData.DampingValue, 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 192, 120, 16 }, "Stiffness (k)", TextFormat("%0.2f", ncEditorData.StiffnessValue), & ncEditorData.StiffnessValue, 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 224, 120, 16 }, "Gravity Scale", TextFormat("%0.2f", ncEditorData.GravityScaleValue), & ncEditorData.GravityScaleValue, 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 312, 120, 16 }, "Gravity", TextFormat("%0.2f", ncEditorData.GravityValue), & ncEditorData.GravityValue, -10, 0);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 344, 120, 16 }, "Gravitation", TextFormat("%0.2f", ncEditorData.GravitationValue), & ncEditorData.GravitationValue, 0, 20);
        if (GuiDropdownBox((Rectangle) { ncEditorData.anchor01.x + 80, ncEditorData.anchor01.y + 80, 152, 32 }, "DYNAMIC;KINEMATIC;STATIC", & ncEditorData.BodyTypeActive, ncEditorData.BodyTypeEditMode)) ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;
    }

	DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, WHITE);

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