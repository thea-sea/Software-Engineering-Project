/*
File: Physics.cpp
Author: w.chongsheng@digipen.edu
Description: Physics components that stores data for which to be used in PhysicSystem.cpp
*/

#pragma once

#include "../Headers/STL_Header.h"
#include "../Engine/System/SystemFrame.h"

namespace Core
{
	//forward declaration
	namespace Object
	{
		class GameObject;
	}
	
	class ObjectFactory : public SystemFrame
	{
	private:
		ObjectFactory* Instance;

		std::set<Object::GameObject*> DeleteList;

	public:		
		ObjectFactory();
		virtual ~ObjectFactory();

		static ObjectFactory* GetInstance()
		{
			static ObjectFactory Instance;
			return &Instance;
		}

		static inline std::unordered_map<std::string, Object::GameObject*> ObjectContainer;
		unsigned int LastObjectID;

		Object::GameObject* Create();

		void AddtoDestroyList(Object::GameObject* Obj);

		void DestroyEverything();

		void Update(const double dt);

		void AddObjects(Object::GameObject* added, std::string i);

		void Init();

		void DeserializeObjects(const char* filename);
	
		void RegisterComponent(std::unordered_map<std::string, Object::GameObject*> ObjectContainer);
	};

	class AssetsManager : public SystemFrame
	{
	public:
		AssetsManager();
		virtual ~AssetsManager();

		void Init();
		void Update(const double dt);
		void RegisterComponent(std::unordered_map<std::string, Object::GameObject*> ObjectContainer);

		void Add_files(std::string path);
		void Go_Deeper(std::filesystem::path path);

		void Add_Assets(const std::string file);
		void Remove_Assets(const std::string file);

		std::vector<std::filesystem::path>& GetFiles();
		void ClearFileContainer();

	private:
		std::vector<std::filesystem::path> files;
	};
}
