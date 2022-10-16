/*
File:		 ECS.cpp
Author:		 w.chongsheng@digipen.edu
Description: This file contains the function definition for Entities and Components
*/

#include "ECS.h"
#include "../Engine/Transform/Transform.h"

//int Core::Entity::entity_count = 0; // Counter for amount of entities created.
//
///*
//	Constructor for a Entity.
//*/
//
//Core::Entity::Entity() :
//	active(true),
//	remove(false)
//{
//	transform = AddComponent<Transform>();
//	//AddComponent<Material>();
//	id = entity_count;
//	++entity_count;
//}
//
///*
//	Destructor for Entity.
//*/
//
//Core::Entity::~Entity()
//{
//	transform->children.clear();
//	transform->parent->Owner()->DetachChild(this);
//	transform->parent = NULL;
//	for (unsigned i = 0; i < components.size(); ++i)
//	{
//		if (components[i] != NULL)
//		{
//			delete components[i];
//			components[i] = NULL;
//		}
//	}
//}
//
///*
//	Returns the identification number for the Entity.
//	To see what components it has.
//*/
//
//const int Core::Entity::GetID()
//{
//	return id;
//}
//
///*
//	Check if the Entity is active.
//*/
//
//bool Core::Entity::IsActive()
//{
//	return active;
//}
//
///*
//	Activates the Entity.
//*/
//
//void Core::Entity::Activate()
//{
//	active = true;
//}
//
///*
//	Deactivates the Entity.
//*/
//
//void Core::Entity::Deactivate()
//{
//	active = false;
//}
//
///*
//	Attaches an Entity a parent Entity.
//*/
//
//void Core::Entity::AddChild(Entity* child)
//{
//	//children.push_back(child);
//	//child->parent = this;
//	transform->children.push_back(child->transform);
//	//transform->parent = transform;
//	child->transform->parent = transform;
//}
//
///*
//	Detaches an Entity from a parent Entity.
//*/
//
//void Core::Entity::DetachChild(Entity* child)
//{
//	for (unsigned i = 0; i < transform->children.size(); ++i)
//	{
//		if (transform->children[i] == child->transform)
//		{
//			transform->children[i]->parent = NULL;
//			transform->children.erase(transform->children.begin() + i);
//			break;
//		}
//	}
//}
//
///*
//	
//*/
//
//void Core::Entity::DetachChild(int number)
//{
//	transform->children[number]->parent = NULL;
//	transform->children.erase(transform->children.begin() + number);
//}
//
//void Core::Entity::DetachAllChild()
//{
//	for (unsigned i = 0; i < transform->children.size(); ++i)
//	{
//		transform->children[i]->parent = NULL;
//	}
//
//	transform->children.clear();
//}
//
//void Core::Entity::DetachParent()
//{
//	transform->parent = NULL;
//}
//
//Core::Component::Component() :
//	active(true),
//	remove(false)
//{
//}


Core::Component::~Component()
{
}

void Core::Component::Init()
{
}

Core::GameObjectProperty* Core::Component::Owner()
{
	return owner;
}

bool Core::Component::IsActive()
{
	return active;
}

void Core::Component::Activate()
{
	active = true;
}

void Core::Component::Deactivate()
{
	active = false;
}

void Core::Component::Remove()
{
	remove = true;
}

//Core::Core::Component::Component()
//{
//
//}
//
//Core::Core::Entity::~Entity()
//{
//	if (!components.empty())
//	{
//		components.clear(); //Doesn't free from heap
//	}
//}
//
//int Core::Core::Entity::getECSID()
//{
//	return ID.second;
//}
//
//std::string Core::Core::Entity::getECSName(Component* components)
//{
//	return ID.first;
//}
//
//void Core::Core::Entity::addComponents(Component* newcomp, std::pair<std::string, int> identity)
//{
//	ID = identity;
//
//	++max_size;
//
//	components[identity.second] = newcomp;
//}
//
//void Core::Core::Entity::removeComponents(std::pair<std::string, int> identity)
//{
//	components.erase(identity.second);
//
//	--max_size;
//}
//
//bool Core::Core::Component::isActive()
//{
//	return active;
//}
//
//void Core::Core::Component::Activate()
//{
//	active = true;
//}
//
//void Core::Core::Component::Deactivate()
//{
//	active = false;
//}