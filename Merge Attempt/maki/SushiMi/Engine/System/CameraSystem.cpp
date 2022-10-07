
#include "CameraSystem.h"
#include "../Camera/Camera2D.h"
#include "../Window/GameWindow.h"

Core::CameraSystem::CameraSystem()
{
}


Core::CameraSystem::~CameraSystem()
{
}

void Core::CameraSystem::RegisterComponents(const std::vector<Entity*>& entities)
{
	for (int i = 0; i < entities.size(); ++i)
	{
		Camera2D* camera = entities[i]->GetComponent<Camera2D>();
		if (camera != NULL)
		{
			cameras.push_back(camera);
		}
	}
}

void Core::CameraSystem::Update(const double dt)
{	
	for (int i = 0; i < cameras.size(); ++i)
	{
		cameras[i]->update(Upfront::Window::win);
	}
}

void Core::CameraSystem::CheckForNewComponents(void)
{

}