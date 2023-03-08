#pragma once

#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "../Engine/System/SystemFrame.h"


namespace Core
{
	/*forward declaration*/
	class Player;
	class Sprite;
	//extern Player* player;
	class ObjectFactory;

	class Window : public SystemFrame
	{
	public:
		Window(int width, int height);
		~Window();

		static Window* GetInstance(int width, int height)
		{
			static Window instance(width,height);
			return &instance;
		}


		float getDelta()
		{
			return static_cast<float>(delta);
		}

		void Init() {}
		void Update(const double) {}
		void RegisterComponent(std::unordered_map<std::string, Object::GameObject*> ObjectContainer) {}

		void Input();

		void Resize();

		void Mainloop();

		/*                                                             input key states
		----------------------------------------------------------------------------- */
		static inline bool keystate_left;
		static inline bool keystate_right;
		static inline bool keystate_up;
		static inline bool keystate_down;
		static inline bool keystate_R;
		static inline bool keystate_M;
		static inline bool keystate_tab;
		static inline bool keystate_space;
		static inline bool keystate_1;
		static inline bool keystate_2;
		static inline bool keystate_3;
		static inline bool keystate_4;
		static inline bool keystate_5;
		static inline bool keystate_6;
		static inline bool keystate_7;
		static inline bool keystate_8;
		static inline bool keystate_9;
		static inline bool keystate_0;
		static inline bool keystate_J;
		static inline bool keystate_K;
		static inline bool keystate_L;
		static inline bool keystate_P;
		static inline bool keystate_T;
		static inline bool keystate_escape;
		static inline bool place_obj;
		static inline bool keystate_W;
		static inline bool keystate_A;
		static inline bool keystate_S;
		static inline bool keystate_D;
		static inline bool keystate_fps;
		static inline bool mouseLeft;
		static inline size_t curr_len = 0;
		static inline std::string realstring = "";

		static inline bool gameIsPaused;
		bool isMenuState;
		
		bool isLevelSelection;
		bool isWalk;
		static inline bool loaded;
		static inline bool isPlayerinSinkhole;

		static inline bool isTut1, isTut2, isLevel1, isLevel2, isLevel3, //Rice Plain Village
		isLevel4, isLevel5, isLevel6, //gunkan village
		isLevel7, isLevel8, isLevel9, //fishing village
		isLevel10, //maki city
		isTestLevel;

		bool isEndingCutscene;

		static inline bool isDialogue;
		static inline int dialogue_style;
		static inline bool show_fps;
		enum class dialogue
		{
			T1 = 0,
			T2,	//1
			L1,	//2
			L2,	//3
			L3,	//4
			L4,	//5
			L5, //6
			L6,	//7
			L7,	//8
			L8,	//9
			L9	//10
		};

		static inline bool isQuestTab;
		static inline bool isWinCondition; //dont use this, Map.cpp has Map:isWin() that returns true or false
		static inline int questProgress;
		static inline int numQuests;


		bool isCutscene;
		static inline int CutscenePage;

		bool isHowToPlay;
		static inline int HowToPlayPage;

		bool isSettings;
		static inline bool isCredits;

		static inline bool timetodeletegrid;

		void SetPaused(bool paused) //take in boolean
		{
			gameIsPaused = paused;

		}

		bool GetPaused(void)//check if game is paused
		{
			return gameIsPaused;
		}

		bool checkWin();

		//for editor
#ifdef EDITOR
		static inline Sprite* ingredient; //for imguiObjectCursor


#endif

		static inline void ImGuiToObjContainer(ObjectFactory*);

		static inline Player* player;
		static inline GLFWwindow* window_ptr; //moved from private to public for access in main.cpp
		/*static inline Sprite* obj;
		static inline Sprite* sp;*/
		/*static inline Sprite* ingredient1;
		static inline Sprite* ingredient2;
		static inline Sprite* goal1;
		static inline Sprite* goal2;
		static inline Sprite* trap;*/
		static inline Sprite* pause_overlay;
		//void ImGuiToObjContainer(ObjectFactory* c);
	


		struct ScreenDimensions {
			static inline int screenwidth;
			static inline int screenheight;
		};

	private:
		int m_width, m_height;
		double starttime, endtime, delta, fps;
	};

	class pseudomain
	{
	public:
		static void draw();
		static void update();
		static void init();
		static void cleanup();
	};
#define Get_Delta()				Window::GetInstance(0, 0)->getDelta()
}
