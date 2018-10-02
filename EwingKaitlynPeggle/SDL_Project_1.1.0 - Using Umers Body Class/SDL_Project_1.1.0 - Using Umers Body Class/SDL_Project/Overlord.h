#ifndef OVERLORD_H
#define OVERLORD_H
class Body;

class Overlord
{
public:
	Overlord();
	~Overlord();
	static void Collision(Body* body1, Body* body2);

};

#endif