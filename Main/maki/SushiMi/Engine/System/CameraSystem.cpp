/*
File: CameraSystem.cpp
Author: w.chongsheng@digipen.edu
Description: Camera System that takes data stored in Camera2D.h and runs it with it's logic.
*/

#include "CameraSystem.h"
#include "../Camera/Camera2D.h"
#include "../Window/GameWindow.h"
#include "../Game Object/GameObject.h"

Core::CameraSystem::CameraSystem()
{
}


Core::CameraSystem::~CameraSystem()
{
}

//void Core::CameraSystem::Init(const std::vector<Entity*>& entities)
//{
//	for (int i = 0; i < entities.size(); ++i)
//	{
//		Camera2D* camera = entities[i]->GetComponent<Camera2D>();
//		if (camera != NULL)
//		{
//			cameras.push_back(camera);
//		}
//	}
//}
void Core::CameraSystem::Init()
{
	// assign address of object of type Object with name "Camera" in objects
	//pgo = &Object::objects.at("Camera");
	//pgo = &Object::GameObject();
}

//store gameobj into container of cameras
void Core::CameraSystem::AddinCameras(Object::GameObject* gameobj)
{
	
	/*for (int i = 0; i < camera.size(); ++i)
	{
		camera[i]
	}*/
	
	if (gameobj->GetObjectProperties()->GetComponent<Camera2D*>(ComponentID::Camera))
	{
		cameras.emplace_back(gameobj->GetObjectProperties()->GetComponent<Camera2D*>(ComponentID::Camera));
	}
}

void Core::CameraSystem::Update(const double dt)
{	
	/*for (int i = 0; i < cameras.size(); ++i)
	{
		cameras[i]->update(Upfront::Window::win);
	}*/
}

void Core::CameraSystem::RegisterComponent(std::unordered_map<std::string, Object::GameObject*> ObjectContainer)
{

}