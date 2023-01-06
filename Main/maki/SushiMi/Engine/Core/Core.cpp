/*!
\file		Core.cpp
\author 	Wong Chong Sheng
\par    	w.chongsheng@digipen.edu
\author 	Aurelia Chong
\par    	email: fei.x@digipen.edu
\date   	16 Nov 2022
\brief		This file contains the MainSystem which runs all the subsystems in the engine.
/*
File:		Core.cpp
Author:		w.chongsheng@digipen.edu
Description: This file contains the MainSystem which runs all the subsystems in the engine.
			 Subsystems consists of Rendering, Transforming, Camera and Factory.
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Core.h"
#include "../Engine/Components/Physics/Physics.h"

//Core::MainSystem* Core::MainSystem::instance = 0; //Singleton of MainSystem.

/*
	Constructor for MainSystem.
*/
Core::MainSystem::MainSystem()
{
	renderer = new Renderer();
	systems.push_back(renderer);

	cameraSystem = new CameraSystem();
	systems.push_back(cameraSystem);

	transformer = new Transformer();
	systems.push_back(transformer);

	physicssystem = new PhysicSystem();
	systems.push_back(physicssystem);

	objfactory = new ObjectFactory();

	inputsystem = new Input();
}

/*
	Destructor for MainSystem.
*/
Core::MainSystem::~MainSystem()
{
	for (auto& sys : systems)
	{
		if (sys != NULL)
		{
			delete sys;
			sys = NULL;
		}
	}
}

/*
	Create Instance of MainSystem.
*/

//Core::MainSystem& Core::MainSystem::Instance()
//{
//	if (instance != 0)
//	{
//		return *instance;
//	}
//
//	instance = new MainSystem();
//	return *instance;
//}

/*
	Register Components to each SubSystem.
*/

void Core::MainSystem::Init()
{
	for (auto& sys : systems)
	{
		sys->Init();

	}
	//creation of obj
	Core::Object::GameObject* temp1 = objfactory->Create();
	Core::Object::GameObject* temp2 = objfactory->Create();
	objfactory->AddObjects(temp1, "Obj Test 1");
	objfactory->AddObjects(temp2, "Obj Test 2");

	//creation of collision objs
	Core::Object::GameObject* Collision1 = objfactory->Create();
	Core::Object::GameObject* Collision2 = objfactory->Create();
	objfactory->AddObjects(Collision1, "CollisionObj1 Test");
	objfactory->AddObjects(Collision2, "CollisionObj2 Test");

	//Core::Object::GameObjectProperty* test = objfactory->ObjectContainer;

	for (auto& i : objfactory->ObjectContainer)
	{
		i.second->GetObjectProperties()->AddComponent(ComponentID::Collision, new Collision());
		static_cast<Collision*>(i.second->GetObjectProperties()->GetComponent(ComponentID::Collision))->GetAABB().min = gfxVector2(0, 0); //set button coordiantes
		static_cast<Collision*>(i.second->GetObjectProperties()->GetComponent(ComponentID::Collision))->GetAABB().max = gfxVector2(100, 100);
	}

}

/*
	Runs the update() function for each SubSystem.
*/

void Core::MainSystem::Update(const double dt)
{
	for (int i = 0; i < systems.size(); ++i)
	{
		systems[i]->Update(dt);
	}

	int mousestate = glfwGetMouseButton(GLHelper::ptr_window, MOUSE_BUTTON_LEFT);

	gfxVector2 mousePos = inputsystem->GetMouse(GLHelper::ptr_window, mousestate); 

	for (auto& i : objfactory->ObjectContainer)
	{
		if (staticPointRect(mousePos, static_cast<Collision*>(i.second->GetObjectProperties()->GetComponent(ComponentID::Collision))->GetAABB()))
		{
			std::cout << "U are clicking" << std::endl;
			/*std::cout << static_cast<Collision*>(i.second->GetObjectProperties()->GetComponent(ComponentID::Collision))->GetAABB().min.x << " , ";
			std::cout << static_cast<Collision*>(i.second->GetObjectProperties()->GetComponent(ComponentID::Collision))->GetAABB().min.y << std::endl;
			std::cout << static_cast<Collision*>(i.second->GetObjectProperties()->GetComponent(ComponentID::Collision))->GetAABB().max.x << " , ";
			std::cout << static_cast<Collision*>(i.second->GetObjectProperties()->GetComponent(ComponentID::Collision))->GetAABB().max.y << std::endl;*/
			std::cout << mousePos.x << ", " << mousePos.y << std::endl;
			break;
			
		}
		
	}

}

/*
	Checks for new Components in each SubSystem.
*/

void Core::MainSystem::RegisterComponent(std::unordered_map<std::string, Object::GameObject*> ObjectContainer)
{
	for (int i = 0; i < systems.size(); ++i)
	{
		systems[i]->RegisterComponent(ObjectContainer);
	}

}

void Core::MainSystem::clear()
{
	for (auto& i : objfactory->ObjectContainer)
	{
		objfactory->AddtoDestroyList(i.second);
	}

	objfactory->DestroyEverything();

	for (auto& i : systems)
	{
		delete i;
	}

	delete objfactory;

	delete inputsystem;
}


//Core::Core::MainSystem::MainSystem()
//{
//}
//
//Core::Core::MainSystem::~MainSystem()
//{
//	for (SystemFrame* i : SubSystems)
//	{
//		if (i != NULL)
//		{
//			delete i;
//			i = NULL;
//		}
//
//		if (elem_pos > 0)
//		{
//			--elem_pos;
//		}
//	}
//}
//
//void Core::Core::MainSystem::Sys_Init()
//{
//	elem_pos = 0;
//}
//
//void Core::Core::MainSystem::update(const float dt)
//{
//
//}
//
//void Core::Core::MainSystem::RegisterEntities(std::array<Entity, 1000> entities)
//{
//	for (SystemFrame* i : SubSystems)
//	{
//		i->RegisterEntities(entities);
//	}
//}