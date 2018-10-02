#ifndef STAR_H
#define STAR_H
#include "Vector.h"
#include "SDL.h"
using namespace MATH;
class Star
{
public:
	Vec3 velocity;
	Vec3 position;
private:
	Vec3 acceleration;
	float mass;
	SDL_Surface *bodyImage;
public:
	Star();
	~Star();
};

#endif