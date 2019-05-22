struct PrecastSkills
{
	WORD Skill;
	BOOL GetSkill;
	BOOL CurSkill;
};

class KeyHooks
{
public:
	KeyHooks();
	~KeyHooks();
	static DWORD WINAPI PrecastThread(LPVOID Parameter);
	static void KeyHook();
	static void __fastcall OnGameKeyDown(byte Key, byte Repeat);
	static LONG __stdcall WindowProc(HWND HWnd, UINT MSG, WPARAM WParam, LPARAM LParam);
};
