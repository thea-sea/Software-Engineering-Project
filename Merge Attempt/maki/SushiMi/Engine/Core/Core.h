#pragma once

/*
File:		 Core.h
Author:		 w.chongsheng@digipen.edu
Description: Header for Core.cpp
*/

#ifndef CORE_H_
#define CORE_H_

#include "../Engine/System/SystemFrame.h"
#include "../Engine//System/Renderer.h"
#include "../Engine/System/Transformer.h"
#include "../Engine/System//CameraSystem.h"

#include <vector>

namespace Core
{
	class MainSystem : public ::SystemFrame
	{
	private:
		MainSystem();
		virtual ~MainSystem();

		static MainSystem* instance;

		std::vector<SystemFrame*> systems;

	public:
		static MainSystem& Instance();

		virtual void RegisterComponents(const std::vector<Core::Entity*>& entities);
		virtual void Update(const double dt);
		virtual void CheckForNewComponents();

		Renderer* renderer;
		Transformer* transformer;
		CameraSystem* cameraSystem;
	};
}

//#include "../Headers/STL_Header.h"
//#include "../Engine/System/SystemFrame.h"
//
////Forward declaration
//class Entity;
//class Component;
//class Renderer;
//class Transformer;
//class CameraSystem;
//
//namespace Core
//{
//	class MainSystem : public SystemFrame
//	{
//	private:
//		MainSystem();
//		~MainSystem();
//		
//		static MainSystem* inst; //Instance of System
//		float PrevTime{}; //Previous Time for dt
//		bool active{}; //Active status
//
//		std::array<SystemFrame*, 1000> SubSystems; //Container for Systems
//	
//	public:
//
//		void Sys_Init();
//
//		void RegisterEntities(const std::array<Entity, 1000> entities);
//
//		void update(const float dt);
//
//		std::vector<std::string> current_components(const std::array<Entity, 1000> &entities);
//
//		void AddComponents(const Component components);
//
//		void AddEntities(const Entity entities);
//
//		void AddSystem(SystemFrame* system);
//		void DeleteSystem();
//
//		Renderer* rendersys;
//		Transformer* tranformsys;
//		CameraSystem* camerasys;
//	};
//}

#endif