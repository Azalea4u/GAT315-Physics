#pragma once

typedef struct ncSpring
{
	struct ncBody* body1;
	struct ncBody* body2;

	float restLength;
	float k; // stiffness

	struct ncSpring* next;
	struct ncSpring* prev;
} ncSpring_t;

extern ncSpring_t* ncSprings;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restlength, float k);
void AddSpring(ncSpring_t* spring);
void DestroySpring(ncSpring_t* spring);
void DestoryAllSprings();

void ApplySpringForce(ncSpring_t* springs);
void ApplySpringForcePosition(struct Vector2 position, struct ncBody* body, float restLength, float k, float damping);