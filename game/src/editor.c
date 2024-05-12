#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 950, 20 };

bool EditorBoxActive = true;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

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
    //
}

void DrawEditor()
{
    if (ncEditorData.DropdownBoxEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive)
    {
        ncEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 600 }, "EDITOR");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 56, 272, 200 }, "Body");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 128, 120, 16 }, "Mass Min", NULL, & ncEditorData.MassMinBarValue, 0, 100);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 160, 120, 16 }, "Mass Max", NULL, & ncEditorData.MassMaxBarValue, 0, 100);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 192, 120, 16 }, "Damping", NULL, & ncEditorData.DampingBarValue, 0, 100);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 224, 120, 16 }, "Gravity Scale", NULL, & ncEditorData.GravityScaleBarValue, 0, 100);
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 288, 272, 120 }, "World");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 312, 120, 16 }, "Gravitation", NULL, & ncEditorData.GravitationBarValue, 0, 100);
        if (GuiDropdownBox((Rectangle) { ncEditorData.anchor01.x + 80, ncEditorData.anchor01.y + 80, 152, 32 }, "DYNAMIC;KINEMATIC;STATIC", & ncEditorData.DropdownBoxActive, ncEditorData.DropdownBoxEditMode)) ncEditorData.DropdownBoxEditMode = !ncEditorData.DropdownBoxEditMode;
    }

    GuiUnlock();
}
