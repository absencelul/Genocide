#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE
#define _WIN32_WINNT 0x600
#define VALIDPTR(x) ( (x) && (!IsBadReadPtr(x,sizeof(x))) )
#define ArraySize(X) (sizeof(X) / sizeof(X[0]))
#define KeyDown(Code)	((GetKeyState(Code) & 0x8000) ? TRUE : FALSE)

#undef UNICODE

#include <windows.h>
#include <algorithm>
#include <ctime>
#include <sys/stat.h>
#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <list>
#include <sstream>
#include <map>
#include <math.h>
#include <process.h>
#include <psapi.h>
#include <shlwapi.h>
#include <string>
#include <time.h>
#include <vector>
#include <winbase.h>
#include <windef.h>
#include <WinNT.h>
#include <WTypes.h>
#include <winuser.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <conio.h>

using namespace std;

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "shlwapi.lib")

#define EXPORT __declspec(dllexport)
#define FASTCALL __fastcall
#define FUNCCALL __cdecl
#define IMPORT __declspec(dllimport)
#define NAKED __declspec(naked)
#define STDCALL __stdcall
#define LPVOID LPVOID

#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

#include "Struct.h"
#include "ArrayEx.h"
#include "Aim.h"
#include "Automap Hooks.h"
#include "Chicken.h"
#include "CMatrix.h"
#include "Commands.h"
#include "CSafeVector.h"
#include "Declarations.h"
#include "EventMessage.h"
#include "Functions.h"
#include "Handler.h"
#include "Helpers.h"
#include "KeyHooks.h"
#include "Math.h"
#include "Missiles.h"
#include "Packet Functions.h"
#include "Packet Received.h"
#include "Packet Send.h"
#include "Pointer.h"
#include "Potion.h"
#include "Reveal.h"
#include "Screen Hooks.h"
#include "Settings.h"
#include "Stubs.h"
#include "SyncObj.h"
#include "TMC.h"
#include "Unit.h"
#include "Variables.h"
#include "Patch.h"
#include "MoveItems.h"
#include "ItemPackets.h"

namespace Inject {
	extern bool Inject(HMODULE hMod, LPVOID lpReserved);
	extern bool Uninject();
	extern void InitializeClass(int Initialize);
}