#pragma once

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

extern bool g_bWireFrameCheck;

extern bool g_bCubeAutoNameCheck;
extern bool g_bObjAutoNameCheck;
extern bool g_bMonAutoNameCheck;
extern bool g_bItemAutoNameCheck;
extern bool g_PopupDelayCheck;

const unsigned int BACKSIZEX = 1280;
const unsigned int BACKSIZEY = 768;

const int iCubeIndex = 17;
const int iObjIndex = 16;
const int iMonIndex = 8;
const int iItemIndex = 11;


enum SCENETYPE { SCENE_STATIC, SCENE_LOGO, SCENE_STAGE, SCENE_END };
enum TOOLTYPE {TOOL_CUBE, TOOL_OBJECT, TOOL_MONSTER, TOOL_ITEM};