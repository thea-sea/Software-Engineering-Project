//******************************************************************************/
/*!
\file		SystemFrame.h
\author 	Wong Chong Sheng
\email    	w.chongsheng@digipen.edu
\date   	25/10/2022
\brief		Header for System base class.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 //******************************************************************************/

#pragma once

#ifndef SYSTEMFRAME_H_
#define SYSTEMFRAME_H_

// Component System
#include <vector>
#include <unordered_map>

namespace Core
{
	namespace Object
	{
		class GameObject;
	}

	class SystemFrame
	{
	public:
		SystemFrame();
		virtual ~SystemFrame();
		virtual void Init() = 0;
		virtual void Update(const double dt) = 0;
		virtual void RegisterComponent(std::unordered_map<unsigned int, Object::GameObject*> ObjectContainer) = 0;
	};
}


//#include "../Headers/STL_Header.h"
//#include "../Components/ECS.h"
//
////Forward Declaration
//class Component;
//class Entity;
//
//namespace Core
//{
//	class SystemFrame
//	{
//	public:
//		unsigned int elem_pos;
//		
//		SystemFrame();
//		virtual ~SystemFrame() = 0;
//
//		virtual void RegisterEntities(const std::array<Entity, 1000> entities) = 0;
//		
//		virtual void update(const float dt) = 0;
//
//		//void SendMsg(Message* messge);
//
//		virtual std::string GetProcessName() = 0;
//
//		virtual void AddComponents(const Component components) = 0;
//
//		virtual void AddEntities(const Entity entities) = 0;
//		virtual void RemoveEntites() = 0;
//	};
//}
#endif