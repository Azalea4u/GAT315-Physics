#pragma once
#include "body.h"

Body* boodies = NULL;
int bodyCount;

Body* CreateBody();
void* DestroyBody(Body* body);