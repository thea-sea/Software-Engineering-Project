#pragma once

#ifndef CAMERASYSTEM_H_
#define CAMERASYSTEM_H_

#include "SystemFrame.h"
#include "../Components/Component.h"


namespace Core
{
	//Forward Declaration
	class Camera2D;

	class CameraSystem : public SystemFrame
	{
	public:
		CameraSystem();
		virtual ~CameraSystem();
		virtual void Init();
		virtual void Update(const double dt);
		virtual void RegisterComponent(std::unordered_map<unsigned int, Object::GameObject*> ObjectContainer);

	private:
		std::vector<Camera2D*> cameras;
	};
}

#endif