#pragma once
#include <raylib.h>

extern ncBody* ncBodies;
extern int bodyCount;
extern Vector2 ncGravity;

ncBody* CreateBody();
void* DestroyBody(ncBody* body);
void DestoryAllBodies();