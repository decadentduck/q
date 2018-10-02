#include "Overlord.h"
#include "math.h"
#include "VMath.h"
#include "Body.h"
#include "MMath.h"
using namespace MATH;

Overlord::Overlord()
{

}

Overlord::~Overlord()
{

}

void Overlord::Collision(Body* body1, Body* body2)
{
	///Find the normal
	///normalize the normal
	///Project vectors onto the normal
	///use conservation of momentum to isolate V1f
	///use epsilon equation to find V1fn and V2fn
	///find change in velocity along the normal
	///find final velocities
	Vec3 v1 = body1->vel;
	Vec3 v2 = body2->vel;
	int m1 = body1->mass;
	int m2 = body2->mass;
	Vec3 p1 = body1->pos;
	Vec3 p2 = body2->pos;

	//epsilon
	float E = 0.9f;

	//Find the normal
	//N = body1 - body2
	Vec3 N = p2 - p1;
	//normalize the normal
	//N = N / |N|
	N = VMath::normalize(N);
	
	//Project vectors onto the normal
	//V1n = V1 * N
	float V1p = VMath::dot(v1, N);
	float V2p = VMath::dot(v2, N);

	//V1i = ((m1 - E m2) V1 + (1 + E) m2 V2)/ m1 + m2
	//V2i = ((m2 - E m1) V2 + (1 + E) m1 V1)/ m1 + m2
	float V1pi = ((m1 - E * m2) * V1p + (1 + E) * m2 * V2p) / (m1 + m2);
	float V2pi = ((m2 - E * m1) * V2p + (1 + E) * m1 * V1p) / (m1 + m2);

	Vec3 V1i = v1 + (V1pi - V1p) * N;
	Vec3 V2i = v2 + (V2pi - V2p) * N;

	//move peg away from the ball a bit
	float tempX = 0;
	float tempY = 0;
	if (V2i.x > 0) tempX = -1.0f;
	else if (V2i.x < 0) tempX = 1.0f;
	if (V2i.y > 0) tempY = -1.0f;
	else if (V2i.y < 0) tempY = 1.0f;

	body2->pos -= Vec3(tempX, tempY, 0.0f);

	//apply velocity to objects
	body1->vel = V1i;
	body2->vel = V2i;
}