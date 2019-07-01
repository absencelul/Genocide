#ifdef VARS
#define ARRAY(Type, Name)		CArrayEx<Type, Type> Name;
#define KEY(Name)				INT Name;
#define TOGGLE(Name)			extern BOOL Name;
#define VAR(Type, Name)			Type Name;
#define VECTOR(Type, Name)		CSafeVector<Type> Name;

#else
#define ARRAY(Type, Name)		extern CArrayEx<Type, Type> Name;
#define KEY(Name)				extern INT Name;
#define TOGGLE(Name)			extern BOOL Name;
#define VAR(Type, Name)			extern Type Name;
#define VECTOR(Type, Name)		extern CSafeVector<Type> Name;

#endif

VAR(HINSTANCE, _dll)
VAR(CHAR, Name[100])
VAR(WNDPROC, OldWndProc)
VAR(CHAR, FilePath[1000])
VAR(CHAR, LocalTime[100])
VAR(int, colorText)
VAR(bool, showStatScreen)
//Classes
//VAR(MoveItems *, Items)

VAR(CRITICAL_SECTION, EX_CRITSECT)

VAR(bool, ClientReady)

//FC Teleport
VAR(bool, FcTele)
VAR(int, FcDistance)

//Autoaim Settings
ARRAY(PTARGETINFO, Players)
VAR(DWORD, CurrentTarget)
VAR(int, TargetKeys[2])
VAR(INT, AutoAim[4])
VAR(INT, hammer[3])
VAR(int, OneKey)

//AuraSwap
VAR(INT, AuraFlash)
VAR(bool, Auras)
VAR(bool, AllowSwap)
VAR(bool, TeleSwap)

//Blinds
VAR(vector<DWORD>, Blinds[2])
VAR(INT, Blindz[2])

//Chicken and Potion Settings
/*tl=0,el=1,tm=2,em=3*/
VAR(int, Chickens[4])
/*pl=0,rl=1,pm=2,rm=3*/
VAR(int, Potions[4])
VAR(ULONGLONG, LastTP)
VAR(bool, Town)
VAR(DWORD, BoTimer)
VAR(ULONGLONG, BoTime)
//VAR(DWORD, PORTALID)
//RESCUE
VAR(bool, RescueWhen[6])
VAR(bool, RescueWhenCursed[10])
VAR(bool, RescueWhenOut[12])

//Commands
VAR(bool, HideCommands)

//Console
VECTOR(LPCONSOLEMESSAGE, ConsoleMessages)
VAR(ULONGLONG, ConsoleDelay)

//Inventory Viewer
VAR(LPUNITANY, ViewingUnit)

//MapHack
VAR(Reveal*, Revealz)
VAR(BOOL, AutoRevealAutomap)
VAR(BOOL, ShrineImages)
VAR(BOOL, MiniShrine)
VAR(BOOL, MonsterLifeBarResists)
VAR(CHAR, StaffTombIndicator[50])
VAR(MYAUTOMAPLAYERMAN, MyAutoMapLayer[100])

VAR(LPCELLFILE, MyShrineCells[2][TOTALSHRINES])
VAR(LPCELLFILE, WaypointCell)
VAR(LPCELLFILE, MinimapObjectCell)

//Missile
VAR(BOOL, ShowMissiles)
VAR(INT, MyMissile)
VAR(INT, FriendMissile)
VAR(INT, NeutralMissile)
VAR(INT, EnemyMissile)

//MISC
/*Open TP/FastTown*/
VAR(int, TownKeys[2])
/*RemoveTPDelay/BlockPortal/RemoveAnim*/
VAR(bool, PortalStates[3])
KEY(Prebuff)
KEY(ViewPlayerGear)
KEY(TeleWalkHostile)
KEY(TeleWalk)
VAR(bool, UseAltScreen)
VAR(bool, ScreenShot)
VAR(bool, TakeScreen)
VAR(DWORD, SSDelay)
VAR(ULONGLONG, SStime)
VAR(int, Reload)
VAR(int, StashCube[2])
VAR(int, killCount)
VAR(int, killCountGame)
VAR(int, Ench)

//Target List
VAR(ULONGLONG, Input)

//Event Messages
VAR(CHAR, ChatClass[50])
VAR(CHAR, ChatClassShort[50])
VAR(CHAR, ChatName[50])
VAR(CHAR, ChatAccount[100])
VAR(CHAR, ChatLevel[50])
VAR(CHAR, ChatLife[50])