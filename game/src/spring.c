#include "spring.h"
#include "body.h"

#include <assert.h>
#include <raymath.h>
#include <string.h>
#include <stdlib.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restlength, float k)
{
	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	assert(spring);

	memset(spring, 0, sizeof(ncSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restlength;
	spring->k = k;

	return spring;
}

void AddSpring(ncSpring_t* spring)
{
	assert(spring);

	// add element to linked list
	spring->prev = NULL;
	spring->next = ncSprings;

	if (ncSprings)
	{
		ncSprings->prev = spring;
	}

	// set head of elements to new element
	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
	assert(spring);

	if (spring->prev) spring->prev->next = spring->next;
	if (spring->next) spring->next->prev = spring->prev;

	if (spring == ncSprings) ncSprings = spring->next;

	free(spring);
}

void DestoryAllSprings()
{
	ncSpring_t* currentString = ncSprings;
	while (currentString)
	{
		ncSpring_t* nextBody = currentString->next;
		free(currentString);
		currentString = nextBody;
	}

	ncSprings = NULL;
}

void ApplySpringForce(ncSpring_t* springs)
{
	for (ncSpring_t* spring = springs; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body2->position, spring->body1->position);
		float distance = Vector2Length(direction);

		float displacement = distance - spring->restLength;
		float force = spring->k * displacement;

		direction = Vector2Normalize(direction);

		ApplyForce(spring->body1, Vector2Scale(direction, force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Scale(direction, -force), FM_FORCE);
	}
}
