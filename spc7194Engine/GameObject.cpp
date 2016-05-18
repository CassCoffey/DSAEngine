#include "GameObject.h"

GameObject::GameObject()
{
	visible = true;
}

GameObject::~GameObject()
{

}

GameObject::GameObject(vec3 location, vec3 rotation, vec3 size, char * iTexture, float mass, ColliderType iCollider, Model* iModel)
{
	transform.location = location;
	transform.rotation = rotation;
	transform.size = size;
	texture = iTexture;
	rigidBody.mass = mass;
	collider = iCollider;
	visible = true;
	model = iModel;
}

void GameObject::updateRigidBody(float deltaTime)
{
	rigidBody.velocity += (rigidBody.force / rigidBody.mass);
	transform.location += rigidBody.velocity * (float)deltaTime;
	rigidBody.force = glm::vec3(0, 0, 0);

	transform.objWorldTransform = glm::translate(transform.location) * glm::yawPitchRoll(transform.rotation.x, transform.rotation.y, transform.rotation.z) * glm::scale(transform.size);
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
		float distanceSquared = 0;
		float radiusSquared = 0;
		if (collider = ColliderType::sphere)
		{
			radiusSquared = pow(transform.size.x, 2);
			// Calculate for X
			if (transform.location.x < (*other).transform.location.x - (*other).transform.size.x)
			{
				distanceSquared += pow(((*other).transform.location.x - (*other).transform.size.x) - transform.location.x, 2);
			}
			else if (transform.location.x > (*other).transform.location.x + (*other).transform.size.x)
			{
				distanceSquared += pow(transform.location.x - ((*other).transform.location.x - (*other).transform.size.x), 2);
			}
			// Calculate for Y
			if (transform.location.y < (*other).transform.location.y - (*other).transform.size.y)
			{
				distanceSquared += pow(((*other).transform.location.y - (*other).transform.size.y) - transform.location.y, 2);
			}
			else if (transform.location.y > (*other).transform.location.y + (*other).transform.size.y)
			{
				distanceSquared += pow(transform.location.y - ((*other).transform.location.y - (*other).transform.size.y), 2);
			}
			// Calculate for Z
			if (transform.location.z < (*other).transform.location.z - (*other).transform.size.z)
			{
				distanceSquared += pow(((*other).transform.location.z - (*other).transform.size.z) - transform.location.z, 2);
			}
			else if (transform.location.z > (*other).transform.location.z + (*other).transform.size.z)
			{
				distanceSquared += pow(transform.location.z - ((*other).transform.location.z - (*other).transform.size.z), 2);
			}
		}
		else
		{
			radiusSquared = pow((*other).transform.size.x, 2);
			// Calculate for X
			if ((*other).transform.location.x < transform.location.x - transform.size.x)
			{
				distanceSquared += pow((transform.location.x - transform.size.x) - (*other).transform.location.x, 2);
			}
			else if ((*other).transform.location.x > transform.location.x + transform.size.x)
			{
				distanceSquared += pow((*other).transform.location.x - (transform.location.x - transform.size.x), 2);
			}
			// Calculate for Y
			if ((*other).transform.location.y < transform.location.y - transform.size.y)
			{
				distanceSquared += pow((transform.location.y - transform.size.y) - (*other).transform.location.y, 2);
			}
			else if ((*other).transform.location.y > transform.location.y + transform.size.y)
			{
				distanceSquared += pow((*other).transform.location.y - (transform.location.y - transform.size.y), 2);
			}
			// Calculate for Z
			if ((*other).transform.location.z < transform.location.z - transform.size.z)
			{
				distanceSquared += pow((transform.location.z - transform.size.z) - (*other).transform.location.z, 2);
			}
			else if ((*other).transform.location.z > transform.location.z + transform.size.z)
			{
				distanceSquared += pow((*other).transform.location.z - (transform.location.z - transform.size.z), 2);
			}
		}

		if (!(distanceSquared < radiusSquared))
		{
			return false;
		}
	}

	return true;
}

std::vector<GameObject> GameObject::shatter()
{
	std::vector<GameObject> tempVec = std::vector<GameObject>();

	//for (int i = 0; i <)

	return tempVec;
}
