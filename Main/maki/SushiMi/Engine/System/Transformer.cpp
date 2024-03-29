/*
File: Transformer.cpp
Author: w.chongsheng@digipen.edu
Description: Transform System which takes data in Transform.h to run in it's logic.
*/

#include "Transformer.h"
#include "../Components/Transform/Transform.h"

namespace Core
{
	Transformer* Transformer::instance;

	Transformer::Transformer()
	{
		instance = nullptr;
	}

	Transformer::~Transformer()
	{
		/*if (instance)
			delete instance;*/
	}

	

	void Transformer::Init()
	{
		/*std::unordered_map<Object::GameObjectProperty*, Transform*>::iterator it;

		for (it = Transforms.begin(); it != Transforms.end(); it++)
		{

		}*/
	}

	void Transformer::Update(const double )
	{
		
		std::unordered_map<std::string, Object::GameObject*>::iterator it = Transforms.begin();

		for (it; it != Transforms.end(); ++it)
		{
			UpdateTransformation(it->second->GetObjectProperties()->GetComponent<Transform>(ComponentID::Transform));
		}
	}

	void Transformer::RegisterComponent(std::unordered_map<std::string, Object::GameObject*> ObjectContainer)
	{
		std::unordered_map<std::string, Object::GameObject*>::iterator it;

		for (it = ObjectContainer.begin(); it != ObjectContainer.end(); ++it)
		{
			Transforms.emplace(it->first, it->second);
		}
	}

	void Transformer::Serialise(const std::string name)
	{

	}

	void Transformer::UpdateTransformation(Transform* )
	{
		
	}
}