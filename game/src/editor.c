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
    GuiLoadStyle("raygui/styles/lavanda/style_lavanda.rgs");

    ncEditorData.GravitationValue = 2;
	ncEditorData.MassMinValue = 0.1f;
	ncEditorData.MassMaxValue = 1;
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor()
{
    if (EditorBoxActive)
    {
        EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 8, anchor01.y + 8, 304, 680 }, "EDITOR");
        GuiProgressBar((Rectangle) { anchor01.x + 88, anchor01.y + 72, 208, 16 }, "Mass Min", NULL, & ncEditorData.MassMinValue, 0, 10);
        GuiProgressBar((Rectangle) { anchor01.x + 88, anchor01.y + 104, 208, 16 }, "Mass Max", NULL, & ncEditorData.MassMaxValue, 0, 10);
        GuiSliderBar((Rectangle) { anchor01.x + 88, anchor01.y + 136, 208, 16 }, "Gravitation", NULL, & ncEditorData.GravitationValue, 0, 100);
    }
}
