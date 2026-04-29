#pragma once

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

/* macOS virtual key codes (from Carbon HIToolbox/Events.h) */
#define KEY_A      0
#define KEY_S      1
#define KEY_D      2
#define KEY_H      4
#define KEY_U      32
#define KEY_I      34
#define KEY_O      31
#define KEY_J      38
#define KEY_K      40
#define KEY_L      37
#define KEY_B      11
#define KEY_N      45
#define KEY_M      46
#define KEY_W      13
#define KEY_SPACE  49
#define KEY_ESCAPE 53
#define KEY_ENTER  36
#define KEY_UP     126
#define KEY_DOWN   125
#define KEY_LEFT   123
#define KEY_RIGHT  124
#define KEY_0      29
#define KEY_1      18
#define KEY_2      19
#define KEY_3      20
#define KEY_4      21
#define KEY_5      23
#define KEY_6      22
#define KEY_7      26
#define KEY_8      28
#define KEY_9      25
#define KEY_KP_0   82
#define KEY_KP_1   83
#define KEY_KP_2   84
#define KEY_KP_3   85
#define KEY_KP_4   86
#define KEY_KP_5   87
#define KEY_KP_6   88
#define KEY_KP_7   89
#define KEY_KP_8   91
#define KEY_KP_9   92
#define KEY_F11    103

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#if __has_include("glfw3.h")
#include "glfw3.h"
#elif __has_include(<GLFW/glfw3.h>)
#include <GLFW/glfw3.h>
#elif __has_include("glfw3/include/mac/glfw3.h")
#include "glfw3/include/mac/glfw3.h"
#else
#error "GLFW header not found. Check include paths."
#endif

/* The unknown key */
#define KEY_UNKNOWN GLFW_KEY_UNKNOWN

/* Printkeys */
#define KEY_SPACE GLFW_KEY_SPACE
#define KEY_APOSTROPHE GLFW_KEY_APOSTROPHE /* ' */
#define KEY_COMMA GLFW_KEY_COMMA		   /* , */
#define KEY_MINUS GLFW_KEY_MINUS		   /* - */
#define KEY_PERIOD GLFW_KEY_PERIOD		   /* . */
#define KEY_SLASH GLFW_KEY_SLASH		   /* / */

#define KEY_0 GLFW_KEY_0
#define KEY_1 GLFW_KEY_1
#define KEY_2 GLFW_KEY_2
#define KEY_3 GLFW_KEY_3
#define KEY_4 GLFW_KEY_4
#define KEY_5 GLFW_KEY_5
#define KEY_6 GLFW_KEY_6
#define KEY_7 GLFW_KEY_7
#define KEY_8 GLFW_KEY_8
#define KEY_9 GLFW_KEY_9

#define KEY_SEMICOLON GLFW_KEY_SEMICOLON /* ; */
#define KEY_EQUAL GLFW_KEY_EQUAL		 /* = */
#define KEY_A GLFW_KEY_A
#define KEY_B GLFW_KEY_B
#define KEY_C GLFW_KEY_C
#define KEY_D GLFW_KEY_D
#define KEY_E GLFW_KEY_E
#define KEY_F GLFW_KEY_F
#define KEY_G GLFW_KEY_G
#define KEY_H GLFW_KEY_H
#define KEY_I GLFW_KEY_I
#define KEY_J GLFW_KEY_J
#define KEY_K GLFW_KEY_K
#define KEY_L GLFW_KEY_L
#define KEY_M GLFW_KEY_M
#define KEY_N GLFW_KEY_N
#define KEY_O GLFW_KEY_O
#define KEY_P GLFW_KEY_P
#define KEY_Q GLFW_KEY_Q
#define KEY_R GLFW_KEY_R
#define KEY_S GLFW_KEY_S
#define KEY_T GLFW_KEY_T
#define KEY_U GLFW_KEY_U
#define KEY_V GLFW_KEY_V
#define KEY_W GLFW_KEY_W
#define KEY_X GLFW_KEY_X
#define KEY_Y GLFW_KEY_Y
#define KEY_Z GLFW_KEY_Z
#define KEY_LEFT_BRACKET GLFW_KEY_LEFT_BRACKET	 /* [ */
#define KEY_BACKSLASH GLFW_KEY_BACKSLASH		 /* \ */
#define KEY_RIGHT_BRACKET GLFW_KEY_RIGHT_BRACKET /* ] */
#define KEY_GRAVE_ACCENT GLFW_KEY_GRAVE_ACCENT	 /* ` */
#define KEY_WORLD_1 GLFW_KEY_WORLD_1			 /* non-US #1 */
#define KEY_WORLD_2 GLFW_KEY_WORLD_2			 /* non-US #2 */

/* Functeys */
#define KEY_ESCAPE GLFW_KEY_ESCAPE
#define KEY_ENTER GLFW_KEY_ENTER
#define KEY_TAB GLFW_KEY_TAB
#define KEY_BACKSPACE GLFW_KEY_BACKSPACE

#define KEY_RIGHT GLFW_KEY_RIGHT
#define KEY_LEFT GLFW_KEY_LEFT
#define KEY_DOWN GLFW_KEY_DOWN
#define KEY_UP GLFW_KEY_UP

#define KEY_F1 GLFW_KEY_F1
#define KEY_F2 GLFW_KEY_F2
#define KEY_F3 GLFW_KEY_F3
#define KEY_F4 GLFW_KEY_F4
#define KEY_F5 GLFW_KEY_F5
#define KEY_F6 GLFW_KEY_F6
#define KEY_F7 GLFW_KEY_F7
#define KEY_F8 GLFW_KEY_F8
#define KEY_F9 GLFW_KEY_F9
#define KEY_F10 GLFW_KEY_F10
#define KEY_F11 GLFW_KEY_F11
#define KEY_F12 GLFW_KEY_F12

#define KEY_KP_0 GLFW_KEY_KP_0
#define KEY_KP_1 GLFW_KEY_KP_1
#define KEY_KP_2 GLFW_KEY_KP_2
#define KEY_KP_3 GLFW_KEY_KP_3
#define KEY_KP_4 GLFW_KEY_KP_4
#define KEY_KP_5 GLFW_KEY_KP_5
#define KEY_KP_6 GLFW_KEY_KP_6
#define KEY_KP_7 GLFW_KEY_KP_7
#define KEY_KP_8 GLFW_KEY_KP_8
#define KEY_KP_9 GLFW_KEY_KP_9
#define KEY_KP_DECIMAL GLFW_KEY_KP_DECIMAL
#define KEY_KP_DIVIDE GLFW_KEY_KP_DIVIDE
#define KEY_KP_MULTIPLY GLFW_KEY_KP_MULTIPLY
#define KEY_KP_SUBTRACT GLFW_KEY_KP_SUBTRACT
#define KEY_KP_ADD GLFW_KEY_KP_ADD
#define KEY_KP_ENTER GLFW_KEY_KP_ENTER
#define KEY_KP_EQUAL GLFW_KEY_KP_EQUAL
#define KEY_LEFT_SHIFT GLFW_KEY_LEFT_SHIFT
#define KEY_LEFT_CONTROL GLFW_KEY_LEFT_CONTROL
#define KEY_LEFT_ALT GLFW_KEY_LEFT_ALT
#define KEY_LEFT_SUPER GLFW_KEY_LEFT_SUPER
#define KEY_RIGHT_SHIFT GLFW_KEY_RIGHT_SHIFT
#define KEY_RIGHT_CONTROL GLFW_KEY_RIGHT_CONTROL
#define KEY_RIGHT_ALT GLFW_KEY_RIGHT_ALT
#define KEY_RIGHT_SUPER GLFW_KEY_RIGHT_SUPER
#define KEY_MENU GLFW_KEY_MENU

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>

#define KEY_PRESSED 0x0100

#define KEY_0 0x30 //  ('0')	0
#define KEY_1 0x31 //  ('1')	1
#define KEY_2 0x32 //  ('2')	2
#define KEY_3 0x33 //  ('3')	3
#define KEY_4 0x34 //  ('4')	4
#define KEY_5 0x35 //  ('5')	5
#define KEY_6 0x36 //  ('6')	6
#define KEY_7 0x37 //  ('7')	7
#define KEY_8 0x38 //  ('8')	8
#define KEY_9 0x39 //  ('9')	9
#define KEY_A 0x41 //  ('A')	A
#define KEY_B 0x42 //  ('B')	B
#define KEY_C 0x43 //  ('C')	C
#define KEY_D 0x44 //  ('D')	D
#define KEY_E 0x45 //  ('E')	E
#define KEY_F 0x46 //  ('F')	F
#define KEY_G 0x47 //  ('G')	G
#define KEY_H 0x48 //  ('H')	H
#define KEY_I 0x49 //  ('I')	I
#define KEY_J 0x4A //  ('J')	J
#define KEY_K 0x4B //  ('K')	K
#define KEY_L 0x4C //  ('L')	L
#define KEY_M 0x4D //  ('M')	M
#define KEY_N 0x4E //  ('N')	N
#define KEY_O 0x4F //  ('O')	O
#define KEY_P 0x50 //  ('P')	P
#define KEY_Q 0x51 //  ('Q')	Q
#define KEY_R 0x52 //  ('R')	R
#define KEY_S 0x53 //  ('S')	S
#define KEY_T 0x54 //  ('T')	T
#define KEY_U 0x55 //  ('U')	U
#define KEY_V 0x56 //  ('V')	V
#define KEY_W 0x57 //  ('W')	W
#define KEY_X 0x58 //  ('X')	X
#define KEY_Y 0x59 //  ('Y')	Y
#define KEY_Z 0x5A //  ('Z')	Z

#define KEY_SHIFT 0x10
#define KEY_CONTROL 0x11

#define KEY_ESCAPE 0x1B

#define KEY_SPACE 0x20
#define KEY_ENTER 0x0D

#define KEY_LEFT 0x25
#define KEY_UP 0x26
#define KEY_RIGHT 0x27
#define KEY_DOWN 0x28

#define KEY_KP_0 0x60
#define KEY_KP_1 0x61
#define KEY_KP_2 0x62
#define KEY_KP_3 0x63
#define KEY_KP_4 0x64
#define KEY_KP_5 0x65
#define KEY_KP_6 0x66
#define KEY_KP_7 0x67
#define KEY_KP_8 0x68
#define KEY_KP_9 0x69

#define KEY_MULTIPLY 0x6A
#define KEY_ADD 0x6B
#define KEY_SEPARATOR 0x6C
#define KEY_SUBTRACT 0x6D
#define KEY_DECIMAL 0x6E
#define KEY_DIVIDE 0x6F

#define KEY_F1 0x70
#define KEY_F2 0x71
#define KEY_F3 0x72
#define KEY_F4 0x73
#define KEY_F5 0x74
#define KEY_F6 0x75
#define KEY_F7 0x76
#define KEY_F8 0x77
#define KEY_F9 0x78
#define KEY_F10 0x79
#define KEY_F11 0x7A
#define KEY_F12 0x7B

#endif

#endif
