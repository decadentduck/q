#include "Body.h" 
#include <SDL_image.h>

Body::Body(char* imageName, float mass_, float radius_ ,Vec3 pos_, Vec3 vel_, Vec3 accel_) 
{
	mass = mass_;
	radius = radius_;
	pos.x = pos_.x;
	pos.y = pos_.y;
	pos.z = pos_.z;

	vel.x = vel_.x;
	vel.y = vel_.y;
	vel.z = vel_.z;

	accel.x = accel_.x;
	accel.y = accel_.y;
	accel.z = accel_.z;
	bodyImage = IMG_Load(imageName);
	if (bodyImage == nullptr)
	{
		/// What should we do?
	}
}

Body::~Body() {} /// There is really nothing to do here - yet!

void Body::Update(const float deltaTime, const float gravity) 
{
	pos.x += (vel.x * deltaTime) + (0.5f * accel.x * (deltaTime * deltaTime));
	pos.y += (vel.y * deltaTime) + (0.5f * (accel.y + gravity) * (deltaTime * deltaTime));
	pos.z += (vel.z * deltaTime) + (0.5f * accel.z * (deltaTime * deltaTime));

	vel.x += accel.x * deltaTime;
	vel.y += (accel.y + gravity) * deltaTime;
	vel.z += accel.z * deltaTime;

	/// Assuming all acceleration comes from an applied force - maybe not in the future - gravity!!
	accel.x = 0.0f;
	accel.y = 0.0f;
	accel.z = 0.0f;
}

void Body::ApplyForce(Vec3 force) 
{
	accel.x = force.x / mass;
	accel.y = force.y / mass;
	accel.z = force.z / mass;
}

SDL_Surface* Body::getImage() 
{
	return bodyImage;
}