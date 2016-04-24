#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

GameObject::GameObject(vec3 location, vec3 rotation, vec3 size, char * iTexture, float mass, ColliderType iCollider)
{
	transform.location = location;
	transform.rotation = rotation;
	transform.size = size;
	texture = iTexture;
	rigidBody.mass = mass;
	collider = iCollider;
}

bool GameObject::collidesWith(const GameObject * other)
{
	if (collider == ColliderType::none || (*other).collider == ColliderType::none)
	{
		return false;
	}
	if (collider == ColliderType::sphere && (*other).collider == ColliderType::sphere)
	{
		float distanceSquared = pow((transform.location.z - (*other).transform.location.z), 2) + pow((transform.location.x - (*other).transform.location.x), 2) + pow((transform.location.y - (*other).transform.location.y), 2);
		float radiusSumSquared = pow((transform.size.x + (*other).transform.size.x), 2);
		if (distanceSquared > radiusSumSquared)
		{
			return false;
		}
	}
	if (collider == ColliderType::aabb && (*other).collider == ColliderType::aabb)
	{
		if (abs(transform.location.x - (*other).transform.location.x) > transform.size.x + (*other).transform.size.x)
		{
			return false;
		}
		if (abs(transform.location.y - (*other).transform.location.y) > transform.size.y + (*other).transform.size.y)
		{
			return false;
		}
		if (abs(transform.location.z - (*other).transform.location.z) > transform.size.z + (*other).transform.size.z)
		{
			return false;
		}
	}
	if (collider != (*other).collider)
	{

	}

	return true;
}
