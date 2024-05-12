#pragma once
#include "raylib.h"

typedef struct ncEditorData
{
    Vector2 anchor01;

    bool EditorBoxActive;
    bool DropdownBoxEditMode;
    int DropdownBoxActive;
    float MassMinBarValue;
    float MassMaxBarValue;
    float DampingBarValue;
    float GravityScaleBarValue;
    float GravitationBarValue;
} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor();