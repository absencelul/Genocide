#pragma once
#include "Patch.h"
#include "Struct.h"

#ifdef _DEFINE_PTRS
#define FUNCPTR(dll, name, callret, args, address) \
	__declspec(naked) callret dll##_##name##args \
	{ \
		static DWORD f##dll##_##name = NULL; \
		if(f##dll##_##name == NULL) \
		{ \
		__asm { pushad } \
		f##dll##_##name = Patch::GetOffset(address); \
		__asm { popad } \
		} \
		__asm jmp [f##dll##_##name] \
	}

#define ASMPTR(dll, name, address) \
	DWORD* Asm_##dll##_##name##(VOID) \
	{ \
		static DWORD f##Asm_##dll##_##name = NULL; \
		if(f##Asm_##dll##_##name## == NULL) \
		{ \
		f##Asm_##dll##_##name## = Patch::GetOffset(address); \
		} \
		return &##f##Asm_##dll##_##name; \
	}

#define VARPTR(dll, name, type, address) \
	type** Var_##dll##_##name##(VOID) \
	{ \
		static DWORD f##Var_##dll##_##name = NULL; \
		if(f##Var_##dll##_##name## == NULL) \
		{ \
		f##Var_##dll##_##name## = Patch::GetOffset(address); \
		} \
		return (type**)&##f##Var_##dll##_##name; \
	}

#else
#define FUNCPTR(dll, name, callret, args, address) extern callret dll##_##name##args;
#define ASMPTR(dll, name, address) extern DWORD* Asm_##dll##_##name##(VOID); static DWORD dll##_##name = *Asm_##dll##_##name##();
#define VARPTR(dll, name, type, address) extern type** Var_##dll##_##name##(VOID); static type* p##_##dll##_##name = (type*)*Var_##dll##_##name##();
#endif

//D2CLIENT FUNCPTR

FUNCPTR(D2CLIENT, PrintGameString, void __fastcall, (wchar_t* wMessage, int nColor), 0x9E3A0) // updated 1.14d
FUNCPTR(D2CLIENT, PrintPartyString, void __fastcall, (wchar_t* wMessage, int nColor), 0x9E5C0)//Updated 1.14d
FUNCPTR(D2CLIENT, DrawManaOrb, void __stdcall, (), 0x97110) // Updated 1.14d
FUNCPTR(D2CLIENT, ExitGame, void __fastcall, (void), 0x4DD60) //Updated 1.14d
FUNCPTR(D2CLIENT, GetGameInfo, GameStructInfo* __stdcall, (), 0x4B7A0) //updated 1.14d
FUNCPTR(D2CLIENT, GetLevelName_I, wchar_t* __fastcall, (DWORD levelId), 0x53E70) //updated 1.14d
FUNCPTR(D2CLIENT, Attack_I, void __stdcall, (AttackStruct* Attack, bool AttackingUnit), 0x61700)//Updated 1.14d
FUNCPTR(D2CLIENT, GetMonsterOwner, DWORD __fastcall, (DWORD nMonsterId), 0x79150) //Updated 1.14d
FUNCPTR(D2CLIENT, DrawEnteringLevelText, void __stdcall, (), 0x55510)
FUNCPTR(D2CLIENT, TestPvpFlag_I, DWORD __fastcall, (DWORD dwUnitId1, DWORD dwUnitId2, DWORD dwFlag), 0xDC440)//Updated 1.14d
FUNCPTR(D2CLIENT, CreateAttack, void __fastcall, (LPUNITANY Unit, DWORD SkillID, DWORD SkillLevel, BOOL Unknown), 0xc6680) //updated 1.14d
FUNCPTR(D2CLIENT, SetUIVar, DWORD __fastcall, (DWORD varno, DWORD howset, DWORD unknown1), 0x55F20)//Updated 1.14d
FUNCPTR(D2CLIENT, GetUiVar_I, DWORD __fastcall, (DWORD dwVarNo), 0x538D0) //Updated 1.14d
FUNCPTR(D2CLIENT, GetSelectedUnit, UnitAny* __stdcall, (), 0x67A10)//Updated 1.14d
FUNCPTR(D2CLIENT, GetMouseXOffset, DWORD __fastcall, (VOID), 0x5AFC0)//Updated 1.14d
FUNCPTR(D2CLIENT, GetMouseYOffset, DWORD __fastcall, (VOID), 0x5AFB0)//Updated 1.14d
FUNCPTR(D2CLIENT, AddAutomapCell, void __fastcall, (AutomapCell* aCell, AutomapCell** node), 0x57B00) //Updated 1.14d
FUNCPTR(D2CLIENT, NewAutomapCell, AutomapCell* FASTCALL, (), 0x57C30) //Updated 1.14d
FUNCPTR(D2CLIENT, RevealAutomapRoom, void __stdcall, (Room1* pRoom1, DWORD dwClipFlag, AutomapLayer* aLayer), 0x58F40) //UPDATED
FUNCPTR(D2CLIENT, GetMonsterTxt, MonsterTxt* FASTCALL, (DWORD MonsterNumber), 0x63832) //Updated 1.14d
FUNCPTR(D2CLIENT, GetDifficulty, BYTE __stdcall, (), 0x4DCD0) //Updated 1.14d
FUNCPTR(D2CLIENT, GetAutomapSize, DWORD __stdcall, (void), 0x5A710) //Updated 1.14d
FUNCPTR(D2CLIENT, InitAutomapLayer_I, AutomapLayer* __fastcall, (DWORD nLayerNo), 0x58D40) //Updated 1.14d
FUNCPTR(D2CLIENT, GetPlayerUnit, UnitAny* __stdcall, (), 0x63DD0)
FUNCPTR(D2CLIENT, GetUnitX, int __fastcall, (UnitAny* pUnit), 0x5ADF0)
FUNCPTR(D2CLIENT, GetUnitY, int __fastcall, (UnitAny* pUnit), 0x5AE20)
//FUNCPTR(D2CLIENT, GameChat_I, BOOL __fastcall, (BYTE* aPacket), 0x5DFC0) // updated 1.14d k 0x26
FUNCPTR(D2MATH, GetDistance, int __stdcall, (UnitAny* Unit, WORD x, WORD y), 0x2417F0)
//FUNCPTR(D2CLIENT, GetUnitHPPercent, DWORD __fastcall, (DWORD dwUnitId), 0x79150) // update
FUNCPTR(D2MATH, GetUnitHPPercent, DWORD __fastcall, (DWORD dwUnitId), 0x79080) // 1.14d
FUNCPTR(D2CLIENT, FindServerSideUnit, UnitAny* __fastcall, (DWORD UnitID, DWORD UnitType), 0x63990) //Updated 1.14d
FUNCPTR(D2CLIENT, DrawRectFrame, void __fastcall, (DWORD Rect), 0x52E50) //Updated 1.14d

//D2CLIENT ASMPTR
ASMPTR(D2CLIENT, GameLoop, 0x4C990) //Updated 1.14d
ASMPTR(D2CLIENT, GetUnitFromId_I, 0x3A5E70) //updated 1.14d
ASMPTR(D2CLIENT, GetUnitFromId_II, 0x63940) // updated 1.14d
ASMPTR(D2CLIENT, InputCall_I, 0x787B0) // updated 1.14d
ASMPTR(D2CLIENT, GetUnitName_I, 0x64A60) //updated 1.14D
ASMPTR(D2CLIENT, AutomapDraw_I, 0x53100) //Updated 1.14d //00453100-BASE
ASMPTR(D2CLIENT, LoadAct_1, 0x59150) // 1.14d
ASMPTR(D2CLIENT, LoadAct_2, 0x62D00) // 1.14d
ASMPTR(D2CLIENT, LoadAct_3, 0x591A0) // 1.14d

//D2CLIENT VARPTRS
VARPTR(D2CLIENT, PlayerUnit, UnitAny*, 0x3A6A70) //Updated 1.14d
VARPTR(D2CLIENT, PlayerUnitList, RosterUnit*, 0x3BB5C0)//Updated 1.14d
//VARPTR(D2CLIENT, ScreenSizeX, DWORD, 0x3D544C) // updated 1.14d
VARPTR(D2CLIENT, ScreenSizeX, DWORD, 0x31146C) // Updated 1.14d //0071146C-BASE
//VARPTR(D2CLIENT, ScreenSizeY, DWORD, 0x3d5450) // updated 1.14d
VARPTR(D2CLIENT, ScreenSizeY, DWORD, 0x311470) // Updated 1.14d //00711470-BASE
VARPTR(D2CLIENT, Ping, DWORD, 0x3A04A4) //Updated 1.14d
VARPTR(D2CLIENT, Skip, DWORD, 0x3A04B0) //Updated 1.14d
VARPTR(D2CLIENT, FPS, DWORD, 0x3BB390)//Updated 1.14d
VARPTR(D2CLIENT, MouseY, DWORD, 0x3A6AAC)//Updated 1.14d //007A6AAC-BASE
VARPTR(D2CLIENT, MouseX, DWORD, 0x3A6AB0)//Updated 1.14d //007A6AB0-BASE
VARPTR(D2CLIENT, StamColor, DWORD, 0x3BEFBA) //updated 1.14d
VARPTR(D2CLIENT, MouseOffsetX, int, 0x3A520C)//Updated 1.14d
VARPTR(D2CLIENT, MouseOffsetY, int, 0x3A5208) //Updated 1.14d
VARPTR(D2CLIENT, Offset, POINT, 0x3A5198) //Updated 1.14d
VARPTR(D2CLIENT, Divisor, int, 0x311254) //Updated 1.14d
VARPTR(D2CLIENT, isWeaponSwitch, int, 0x3BCC4C) //Updated 1.14d
VARPTR(D2CLIENT, AutomapOn, DWORD, 0x3A27E8) //Updated 1.14d
VARPTR(D2CLIENT, AutomapLayerList, LPAUTOMAPLAYER, 0x3A5160) //Updated 1.14d
VARPTR(D2CLIENT, AutomapLayer, LPAUTOMAPLAYER, 0x3A5164) //Updated 1.14d
VARPTR(D2CLIENT, UIMode, int, 0x3A27C0) //BASE - 007A27C0
VARPTR(D2CLIENT, ExpCharFlag, DWORD, 0x3A04F4) // Updated 1.14d //007A04F4-BASE

//D2COMMON FUNCPTR
FUNCPTR(D2COMMON, GetTownLevel, int __stdcall, (DWORD dwActNo), 0x21AB70) //updated 1.14d
FUNCPTR(D2COMMON, GetLevelNoByRoom, int __stdcall, (Room1* ptRoom), 0x21A1B0)
FUNCPTR(D2COMMON, GetUnitStat, DWORD __stdcall, (UnitAny* pUnit, DWORD dwStat, DWORD dwStat2), 0x225480) //Updated 1.14d
FUNCPTR(D2COMMON, GetUnitState, INT STDCALL, (LPUNITANY Unit, DWORD State), 0x239DF0) // updated 1.14d
FUNCPTR(D2COMMON, CheckCollision, DWORD STDCALL, (LPROOM1 pRoom, DWORD X, DWORD Y, DWORD dwBitMask), 0x24CB30) // updated 1.14d
FUNCPTR(D2COMMON, GetSkillLevel, INT __stdcall, (UnitAny* pUnit, Skill* pSkill, BOOL bTotal), 0x2442A0) //Updated 1.14d
FUNCPTR(D2COMMON, GetItemByBodyLoc, LPUNITANY __stdcall, (Inventory* pInventory, int aLoc), 0x23BDE0) //Updated 1.14d
FUNCPTR(D2COMMON, MapToAbsScreen, void __stdcall, (long* pX, long* pY), 0x243260)//Updated 1.14d
FUNCPTR(D2COMMON, AbsScreenToMap, void __stdcall, (long* ptMouseX, long* ptMouseY), 0x243510) //Updated 1.14d
FUNCPTR(D2COMMON, GetSuperUniqueTxt, LPSUPERUNIQUETXT STDCALL, (DWORD monno), 0x227410) // Updated 1.14d //00627410-BASE
FUNCPTR(D2COMMON, AddRoomData, void __stdcall, (Act* ptAct, int LevelId, int Xpos, int Ypos, Room1* pRoom), 0x21A070) //Updated 1.14d //0061A070-BASE
FUNCPTR(D2COMMON, RemoveRoomData, void __stdcall, (Act* ptAct, int LevelId, int Xpos, int Ypos, Room1* pRoom), 0x21A0C0) //Updated 1.14d //0061A0C0-BASE
FUNCPTR(D2COMMON, InitLevel, void __stdcall, (Level* pLevel), 0x2424A0)//Updated 1.14d //006424A0-BASE
FUNCPTR(D2COMMON, GetObjectTxt, ObjectTxt* __stdcall, (DWORD objno), 0x240E90) // Updated 1.14d
FUNCPTR(D2COMMON, GetLevel, Level* __fastcall, (ActMisc* pMisc, DWORD dwLevelNo), 0x242AE0) //Updated 1.14d //00642AE0-BASE
FUNCPTR(D2COMMON, GetLayer, AutomapLayer2* __fastcall, (DWORD dwLevelNo), 0x21E470) //Updated 1.14d
FUNCPTR(D2COMMON, GetTreasureClasses, LPTREASURECLASS STDCALL, (DWORD tcno, DWORD lvlno), 0x221330) // Updated 1.14d
FUNCPTR(D2COMMON, GetItemText, ItemTxt* __stdcall, (DWORD itemno), 0x2335F0)//Updated 1.14d
FUNCPTR(D2COMMON, GetLevelText, LevelTxt* __stdcall, (DWORD levelno), 0x21DB70)
FUNCPTR(D2COMMON, LoadAct, Act* __stdcall, (DWORD ActNumber, DWORD MapId, DWORD Unk, DWORD Unk_2, DWORD Unk_3, DWORD Unk_4, DWORD TownLevelId, DWORD Func_1, DWORD Func_2), 0x2194A0)
FUNCPTR(D2COMMON, UnloadAct, unsigned __stdcall, (Act* pAct), 0x21AFD0)
//FUNCPTR(D2COMMON, GetLeftSkill, Skill* __stdcall, (UnitAny* pUnit), 0x220190) //1.14d
//FUNCPTR(D2COMMON, GetRightSkill, Skill* __stdcall, (UnitAny* pUnit), 0x2201D0) //1.14d

//D2GFX
FUNCPTR(D2GFX, GetHwnd, HWND __stdcall, (void), 0xF59A0) //Updated 1.14d
FUNCPTR(D2GFX, DrawLine, void __stdcall, (int X1, int Y1, int X2, int Y2, DWORD dwColor, DWORD dwUnk), 0xF6380) //Updated 1.14d
FUNCPTR(D2GFX, DrawRectangle, void __stdcall, (int X1, int Y1, int X2, int Y2, DWORD dwColor, DWORD dwTrans), 0xF6300) //Updated 1.14d //004F6300-BASE
FUNCPTR(D2GFX, DrawAutomapCell, void __stdcall, (CellContext* context, DWORD xpos, DWORD ypos, RECT* cliprect, DWORD bright), 0xF6510) //Updated 1.14d

//D2CMP
FUNCPTR(D2CMP, InitCellFile, VOID __stdcall, (LPVOID File, CellFile** Out, LPSTR SourceFile, DWORD Line, DWORD FileVersion, LPSTR Filename), 0x201340)//Updated 1.14d
FUNCPTR(D2CMP, DeleteCellFile, VOID __stdcall, (LPCELLFILE File), 0x201a50) //Updated 1.14d

//D2NET FUNCPTR
FUNCPTR(D2NET, SendPacket, bool __stdcall, (size_t aLen, DWORD arg1, BYTE* aPacket), 0x12AE50) //Updated 1.14d
FUNCPTR(D2NET, ReceivePacket_I, void __stdcall, (BYTE* aPacket, DWORD aLen), 0x12B920) //Updated 1.14d
FUNCPTR(D2NET, ReceivePacket, void __fastcall, (BYTE* aPacket, DWORD aLen), 0x12AEB0) //Updated 1.14d

//D2LAUNCH VARPTR
VARPTR(D2LAUNCH, BnData, BnetData*, 0x37165C) //Updated 1.14b //0077165C-BASE

FUNCPTR(D2WIN, GetTextWidth, int __fastcall, (const wchar_t* wStr), 0x101820) // updated 1.14d
FUNCPTR(D2WIN, GetTextWidthFileNo, DWORD __fastcall, (wchar_t* wStr, DWORD* dwWidth, DWORD* dwFileNo), 0x102520) //Updated 1.14d //00502520-BASE
FUNCPTR(D2WIN, TakeScreenshot, void __fastcall, (), 0xFA7A0) //Updated 1.14d
FUNCPTR(D2WIN, SetTextSize, DWORD __fastcall, (DWORD dwSize), 0x102EF0) //Updated 1.14d //00502EF0-BASE
FUNCPTR(D2WIN, DrawText, bool __fastcall, (const wchar_t* wStr, int xPos, int yPos, DWORD dwColor, DWORD dwUnk), 0x102320) //Updated 1.14d //00502320-BASE
FUNCPTR(D2WIN, DrawTextEx, void __fastcall, (const wchar_t* wStr, int X, int Y, int nColor, int Centered, int TransLvl), 0x102360) //updated 1.14d
FUNCPTR(D2WIN, DrawRectangledText, void __fastcall, (const wchar_t* wStr, int X, int Y, int nRectColor, int nRectTrans, int nColor), 0x103000)
ASMPTR(D2WIN, WindowProc_I, 0x65DE9) // updated 1.14b

#undef FUNCPTR
#undef ASMPTR
#undef VARPTR

#define GetLevel(LevelNo)					(D2COMMON_GetLevelNoByRoom(LevelNo))
#define GetTownLevel(ActNo)					(D2COMMON_GetTownLevel(ActNo))
#define GetUIVar(UI)						(Stubs::GetUIVarSTUB(UI))
#define GetUnit(ID, Type)					(Stubs::GetUnitSTUB(ID, Type))
#define GetUnitName(X)						(Stubs::GetUnitNameSTUB((DWORD)X))
#define GetUnitStat(Unit, Stat)				(D2COMMON_GetUnitStat(Unit, Stat, 0))
#define GetUnitState(Unit, State)			(D2COMMON_GetUnitState(Unit, State))
#define Me									(*p_D2CLIENT_PlayerUnit)
#define FPS									(*p_D2CLIENT_FPS)
#define Ping								(*p_D2CLIENT_Ping)
#define Skip								(*p_D2CLIENT_Skip)
#define MouseX								(*p_D2CLIENT_MouseX)
#define MouseY								(*p_D2CLIENT_MouseY)
#define PvPStatus(ID1,ID2,Flag)				(D2CLIENT_TestPvpFlag_I(ID1,ID2,Flag))
#define AutoMapLayer						(*p_D2CLIENT_AutomapLayer)