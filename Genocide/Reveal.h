#include "CMatrix.h"

struct LevelList {
	unsigned int levelId;
	unsigned int x, y, act;
};

void ResetRevealed();
void RevealAutomap();
void RevealAct(int act);
void RevealLevel(Level* pLevel);
void RevealRoom(Room2* pRoom);
Level* GetLevels(Act* pAct, DWORD dwLevelNo);
AutomapLayer* InitAutomapLayer(DWORD dwLayer);

VOID DeleteShrines();
VOID InitShrines();

VOID FASTCALL OverrideShrine(LPAUTOMAPCELL New, LPAUTOMAPCELL Current);
//VOID FASTCALL ItemName(WCHAR * Name, LPUNITANY Item);
//VOID FASTCALL SocketViewtableCallRoutine(LPUNITANY Unit, DWORD Argument, VOID(STDCALL * Function)(LPUNITANY, DWORD));
//VOID STDCALL SocketViewtable(VOID(STDCALL * Function)(LPUNITANY, DWORD), LPUNITANY Unit, DWORD Argument);
//LPUNITANY ViewInventory();
//BOOL ViewInventorySocketProtection();