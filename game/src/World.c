#include "World.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int bodyCount = 0;

ncBody* CreateBody()
{
	ncBody* newBody = (ncBody*)malloc(sizeof(ncBody));
	assert(newBody);

	memset(newBody, 0, sizeof(ncBody));

	// add body to linked list
	newBody->prev = NULL;
	newBody->next = ncBodies;

	if (ncBodies)
	{
		ncBodies->prev = newBody;
	}

	ncBodies = newBody;
	bodyCount++;

	return newBody;
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
