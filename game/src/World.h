#pragma once
#include "body.h"

extern ncBody* ncBodies;
extern int bodyCount;

ncBody* CreateBody();
void* DestroyBody(ncBody* body);
void DestoryAllBodies();