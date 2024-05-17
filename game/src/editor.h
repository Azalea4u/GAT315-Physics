#pragma once
#include "raylib.h"

typedef struct ncEditorData
{
    Vector2 anchor01;
     
    bool EditorBoxActive;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float MassValue;
    float DampingValue;
    float GravityScaleValue;
    float StiffnessValue;
	float GravityValue;
    float GravitationValue;
} ncEditorData_t;

extern ncEditorData_t ncEditorData;
extern bool ncEditorIntersect;

extern bool ncEditorActive;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 position);

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);