#pragma once
<<<<<<< HEAD

=======
/*!
@file    Physics.h
@author	 Aurelia Chong
\par	 email: fei.x@digipen.edu
		 Manages game physics such as acceleration and decelleration

 /*		Copyright(C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
*//*__________________________________________________________________________*/
>>>>>>> M3-submission-branch
#include "../Headers/STL_Header.h"
#include "../Component.h"
#include "../Headers/Math_Header.h"

namespace Core
{
	class Physics : public Component
	{
	private:
		//Physics* instance;

	public:
		Physics();
		virtual ~Physics();

		void Init();
		void Serialise(const std::string name);
<<<<<<< HEAD
=======
		void Deserialise(const std::string name);
>>>>>>> M3-submission-branch
		
		float mass{};
		gfxVector2 velocity{};
		gfxVector2 position{};

		float GetMass();
		gfxVector2 GetVelocity();
		gfxVector2 GetPosition();
	};
}