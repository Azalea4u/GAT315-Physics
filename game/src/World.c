#include "body.h"
#include "World.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int bodyCount = 0;
Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType)
{
	ncBody* newBody = (ncBody*)malloc(sizeof(ncBody));
	assert(newBody);

	memset(newBody, 0, sizeof(ncBody));
	newBody->position = position;
	newBody->mass = mass;
	newBody->inverseMass = (bodyType == BT_DYNAMIC) ? 1.0f / mass : 0;
	newBody->type = bodyType;

	return newBody;
}

void AddBody(ncBody* body)
{
	assert(body);

	// add element to linked list
	body->prev = NULL;
	body->next = ncBodies;

	if (ncBodies) ncBodies->prev = body;

	// set head of elements to new element
	ncBodies = body;

	bodyCount++;
}

void* DestroyBody(ncBody* body)
{
	if (body->prev)
	{
		body->prev->next = body->next;
	}
	else
	{
		ncBodies = body->next;
	}

	if (body->next)
	{
		body->next->prev = body->prev;
	}

	free(body);
	bodyCount--;
}

void DestoryAllBodies()
{
	ncBody* currentBody = ncBodies;
	while (currentBody)
	{
		ncBody* nextBody = currentBody->next;
		free(currentBody);
		currentBody = nextBody;
	}

	ncBodies = NULL;
	bodyCount = 0;
}
