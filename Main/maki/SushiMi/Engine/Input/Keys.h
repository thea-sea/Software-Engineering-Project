#pragma once
<<<<<<< HEAD
=======
/******************************************************************************/
/*!
\file		Keys.h
\author 	Aurelia Chong
\par    	email: fei.x@digipen.edu
\date   	16 Nov 2022
\brief		This file contains enum of keys
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
>>>>>>> M3-submission-branch
#ifndef KEYS_H__
#define KEYS_H__

namespace Core
{
	enum KEY
	{
		KEY_SPACE = 0,
		KEY_APOSTROPHE = 1,
		KEY_PLUS = 2,
		KEY_COMMA = 3,
		KEY_MINUS = 4,
		KEY_PERIOD = 5,
		KEY_SLASH = 6,
		KEY_0 = 7,
		KEY_1 = 8,
		KEY_2 = 9,
		KEY_3 = 10,
		KEY_4 = 11,
		KEY_5 = 12,
		KEY_6 = 13,
		KEY_7 = 14,
		KEY_8 = 15,
		KEY_9 = 16,
		KEY_SEMICOLON = 17,
		KEY_EQUAL = 18,
		KEY_A = 19,
		KEY_B = 20,
		KEY_C = 21,
		KEY_D = 22,
		KEY_E = 23,
		KEY_F = 24,
		KEY_G = 25,
		KEY_H = 26,
		KEY_I = 27,
		KEY_J = 28,
		KEY_K = 29,
		KEY_L = 30,
		KEY_M = 31,
		KEY_N = 32,
		KEY_O = 33,
		KEY_P = 34,
		KEY_Q = 35,
		KEY_R = 36,
		KEY_S = 37,
		KEY_T = 38,
		KEY_U = 39,
		KEY_V = 40,
		KEY_W = 41,
		KEY_X = 42,
		KEY_Y = 43,
		KEY_Z = 44,
		KEY_LEFT_BRACKET = 45,
		KEY_BACKSLASH = 46,
		KEY_RIGHT_BRACKET = 47,
		KEY_a = 48,
		KEY_b = 49,
		KEY_c = 50,
		KEY_d = 51,
		KEY_e = 52,
		KEY_f = 53,
		KEY_g = 54,
		KEY_h = 55,
		KEY_i = 56,
		KEY_j = 57,
		KEY_k = 58,
		KEY_l = 59,
		KEY_m = 60,
		KEY_n = 61,
		KEY_o = 62,
		KEY_p = 63,
		KEY_q = 64,
		KEY_r = 65,
		KEY_s = 66,
		KEY_t = 67,
		KEY_u = 68,
		KEY_v = 69,
		KEY_w = 70,
		KEY_x = 71,
		KEY_y = 72,
		KEY_z = 73,
		KEY_ESCAPE = 74,
		KEY_ENTER,
		KEY_TAB,
		KEY_BACKSPACE,
		KEY_INSERT,
		KEY_DELETE,
		KEY_RIGHT,
		KEY_LEFT,
		KEY_DOWN,
		KEY_UP,
		KEY_PAGE_UP,
		KEY_PAGE_DOWN,
		KEY_HOME,
		KEY_END,
		KEY_F1 = 75,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_UNDEFINED = 999,
	};

	enum MOUSE
	{
		MOUSE_BUTTON_LEFT = 0,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE,
		MOUSE_UNDEFINED = 999,
	};

	enum KEY_STATE
	{
		KEY_STATE_RELEASE,
		KEY_STATE_PRESS,
		KEY_STATE_REPEAT,
		KEY_STATE_UNDEFINED = 999,
	};
}

#endif