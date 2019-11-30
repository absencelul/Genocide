#include "Hack.h"

Settings::Settings()
{
}

Settings::~Settings()
{
}

void Settings::LoadConfig()
{
	CHAR File[MAX_PATH], Returned[100000];
	GetModuleDirectory();

	strncpy_s(File, sizeof(File), FilePath, sizeof(File));

	strcat_s(File, "\\Config.ini");

	CONFIGURATION C[] =
	{
		{ "AUTO-AIM", "Aim", INTEGER, &AutoAim },
		{ "AUTO-AIM", "OneKey", INTEGER, &OneKey },
		{ "AUTO-AIM", "Hammer", INTEGER, &hammer },
		{ "AUTO-AIM", "Toggle AuraFlash", INTEGER, &AuraFlash },
		{ "AUTO-AIM", "AuraFlash", BOOLEAN, &Auras },
		{ "AUTO-AIM", "SwitchTarget", INTEGER, &TargetKeys },

		{ "CONSOLE", "Console Delay", INTEGER, &ConsoleDelay },

		{ "CHICKEN", "Portal States", BOOLEAN, &PortalStates },
		{ "CHICKEN", "Chickens", INTEGER, &Chickens },
		{ "CHICKEN", "RescueWhen", CURSE, &RescueWhen },
		{ "CHICKEN", "RescueWhenCursed", CURSE, &RescueWhenCursed },
		{ "CHICKEN", "RescueWhenOut", CURSE, &RescueWhenOut },
		{ "CHICKEN", "Potions", INTEGER, &Potions },
		{ "CHICKEN", "Town Hotkeys", INTEGER, &TownKeys },

		{ "MISC", "Reload", INTEGER, &Reload },
		{ "MISC", "Prebuff", INTEGER, &Prebuff },
		{ "MISC", "ScreenShot", BOOLEAN, &ScreenShot },
		{ "MISC", "ScreenShot Delay", INTEGER, &SSDelay },
		{ "MISC", "Show Items In ScreenShot", BOOLEAN, &UseAltScreen },
		{ "MISC", "View Players Gear Hotkey", INTEGER, &ViewPlayerGear },
		{ "MISC", "TeleWalk Hotkey", INTEGER, &TeleWalk },
		{ "MISC", "TeleWalk Hostile Hotkey", INTEGER, &TeleWalkHostile },
		{ "MISC", "FC Teleport Distance", INTEGER, &FcDistance },
		{ "MISC", "Chat Color", INTEGER, &colorText },
		{ "MISC", "Show Extra Stats", BOOLEAN, &showStatScreen },

		{ "REVEAL", "MapHack", BOOLEAN, &AutoRevealAutomap },
		{ "REVEAL", "Shrine Images", BOOLEAN, &ShrineImages },

		{ "REVEAL", "Show Missiles", BOOLEAN, &ShowMissiles },
		{ "REVEAL", "My Missile Color", INTEGER, &MyMissile },
		{ "REVEAL", "Neutral Missile Color", INTEGER, &NeutralMissile },
		{ "REVEAL", "Friend Missile Color", INTEGER, &FriendMissile },
		{ "REVEAL", "Enemy Missile Color", INTEGER, &EnemyMissile },

		{ "REVEAL", "Show Monsters", BOOLEAN, &show_monsters },
		{ "REVEAL", "Normal Monster Color", INTEGER, &normal_monster },
		{ "REVEAL", "Boss Monster Color", INTEGER, &boss_monster },
		{ "REVEAL", "Champion Monster Color", INTEGER, &champion_monster },
		{ "REVEAL", "Minion Monster Color", INTEGER, &minion_monster }
	};

	for (INT i = 0; i < ArraySize(C); i++)
	{
		GetPrivateProfileStringA(C[i].Section, C[i].Name, NULL, Returned, sizeof(Returned), File);

		if (C[i].Type == BOOLEAN)
			SetBoolValues(Returned, reinterpret_cast<bool*>(C[i].Variable));

		else if (C[i].Type == INTEGER)
			SetKeyValues(Returned, reinterpret_cast<INT*>(C[i].Variable));

		else if (C[i].Type == BLIND)
			ParseBlind(Returned);

		else if (C[i].Type == CURSE)
			SetCurseValues(Returned, reinterpret_cast<bool*>(C[i].Variable));
	}
}

void Settings::Config()
{
	CHAR File[MAX_PATH], Returned[100000];
	GetModuleDirectory();

	strncpy_s(File, sizeof(File), FilePath, sizeof(File));

	strcat_s(File, "\\Config.ini");

	CONFIGURATION C[] =
	{
		{ "AUTO-AIM", "Blind", INTEGER, &Blindz },
		{ "AUTO-AIM", "Blinds", BLIND, NULL },
	};

	for (INT i = 0; i < ArraySize(C); i++)
	{
		GetPrivateProfileStringA(C[i].Section, C[i].Name, NULL, Returned, sizeof(Returned), File);

		if (C[i].Type == BOOLEAN)
			SetBoolValues(Returned, reinterpret_cast<bool*>(C[i].Variable));

		else if (C[i].Type == INTEGER)
			SetKeyValues(Returned, reinterpret_cast<INT*>(C[i].Variable));

		else if (C[i].Type == BLIND)
			ParseBlind(Returned);
	}
}

void Settings::GetModuleDirectory()
{
	GetModuleFileNameA(_dll, FilePath, MAX_PATH);

	PathRemoveFileSpecA(FilePath);

	strcat_s(FilePath, sizeof(FilePath), "\\");
}

bool Settings::Bool(LPSTR Value)
{
	CharLower(Value);

	if (!_stricmp(Value, "on") || !_stricmp(Value, "true"))
		return true;

	if (!_stricmp(Value, "off") || !_stricmp(Value, "false"))
		return false;

	return false;
}

int Settings::Integer(LPSTR Value)
{
	if (!_stricmp("Nearest", Value)) return 0;
	if (!_stricmp("Target", Value)) return 1;

	if (!_stricmp("Off", Value)) return 0;
	if (!_stricmp("Angle", Value)) return 1;
	if (!_stricmp("Mouse", Value)) return 2;

	if (!_stricmp("A", Value)) return 'A';
	if (!_stricmp("B", Value)) return 'B';
	if (!_stricmp("C", Value)) return 'C';
	if (!_stricmp("D", Value)) return 'D';
	if (!_stricmp("E", Value)) return 'E';
	if (!_stricmp("F", Value)) return 'F';
	if (!_stricmp("G", Value)) return 'G';
	if (!_stricmp("H", Value)) return 'H';
	if (!_stricmp("I", Value)) return 'I';
	if (!_stricmp("J", Value)) return 'J';
	if (!_stricmp("K", Value)) return 'K';
	if (!_stricmp("L", Value)) return 'L';
	if (!_stricmp("M", Value)) return 'M';
	if (!_stricmp("N", Value)) return 'N';
	if (!_stricmp("O", Value)) return 'O';
	if (!_stricmp("P", Value)) return 'P';
	if (!_stricmp("Q", Value)) return 'Q';
	if (!_stricmp("R", Value)) return 'R';
	if (!_stricmp("S", Value)) return 'S';
	if (!_stricmp("T", Value)) return 'T';
	if (!_stricmp("U", Value)) return 'U';
	if (!_stricmp("V", Value)) return 'V';
	if (!_stricmp("W", Value)) return 'W';
	if (!_stricmp("X", Value)) return 'X';
	if (!_stricmp("Y", Value)) return 'Y';
	if (!_stricmp("Z", Value)) return 'Z';

	if (!_stricmp("VK_0", Value)) return '0';
	if (!_stricmp("VK_1", Value)) return '1';
	if (!_stricmp("VK_2", Value)) return '2';
	if (!_stricmp("VK_3", Value)) return '3';
	if (!_stricmp("VK_4", Value)) return '4';
	if (!_stricmp("VK_5", Value)) return '5';
	if (!_stricmp("VK_6", Value)) return '6';
	if (!_stricmp("VK_7", Value)) return '7';
	if (!_stricmp("VK_8", Value)) return '8';
	if (!_stricmp("VK_9", Value)) return '9';

	if (!_stricmp("VK_LBUTTON", Value)) return VK_LBUTTON;
	if (!_stricmp("VK_MBUTTON", Value)) return VK_MBUTTON;
	if (!_stricmp("VK_RBUTTON", Value)) return VK_RBUTTON;

	if (!_stricmp("VK_NUMPAD0", Value)) return VK_NUMPAD0;
	if (!_stricmp("VK_NUMPAD1", Value)) return VK_NUMPAD1;
	if (!_stricmp("VK_NUMPAD2", Value)) return VK_NUMPAD2;
	if (!_stricmp("VK_NUMPAD3", Value)) return VK_NUMPAD3;
	if (!_stricmp("VK_NUMPAD4", Value)) return VK_NUMPAD4;
	if (!_stricmp("VK_NUMPAD5", Value)) return VK_NUMPAD5;
	if (!_stricmp("VK_NUMPAD6", Value)) return VK_NUMPAD6;
	if (!_stricmp("VK_NUMPAD7", Value)) return VK_NUMPAD7;
	if (!_stricmp("VK_NUMPAD8", Value)) return VK_NUMPAD8;
	if (!_stricmp("VK_NUMPAD9", Value)) return VK_NUMPAD9;

	if (!_stricmp("F1", Value)) return VK_F1;
	if (!_stricmp("VK_F1", Value)) return VK_F1;

	if (!_stricmp("F2", Value)) return VK_F2;
	if (!_stricmp("VK_F2", Value)) return VK_F2;

	if (!_stricmp("F3", Value)) return VK_F3;
	if (!_stricmp("VK_F3", Value)) return VK_F3;

	if (!_stricmp("F4", Value)) return VK_F4;
	if (!_stricmp("VK_F4", Value)) return VK_F4;

	if (!_stricmp("F5", Value)) return VK_F5;
	if (!_stricmp("VK_F5", Value)) return VK_F5;

	if (!_stricmp("F6", Value)) return VK_F6;
	if (!_stricmp("VK_F6", Value)) return VK_F6;

	if (!_stricmp("F7", Value)) return VK_F7;
	if (!_stricmp("VK_F7", Value)) return VK_F7;

	if (!_stricmp("F8", Value)) return VK_F8;
	if (!_stricmp("VK_F8", Value)) return VK_F8;

	if (!_stricmp("F9", Value)) return VK_F9;
	if (!_stricmp("VK_F9", Value)) return VK_F9;

	if (!_stricmp("F10", Value)) return VK_F10;
	if (!_stricmp("VK_F10", Value)) return VK_F10;

	if (!_stricmp("F11", Value)) return VK_F11;
	if (!_stricmp("VK_F11", Value)) return VK_F11;

	if (!_stricmp("F12", Value)) return VK_F12;
	if (!_stricmp("VK_F12", Value)) return VK_F12;

	if (!_stricmp("F13", Value)) return VK_F13;
	if (!_stricmp("VK_F13", Value)) return VK_F13;

	if (!_stricmp("F14", Value)) return VK_F14;
	if (!_stricmp("VK_F14", Value)) return VK_F14;

	if (!_stricmp("F15", Value)) return VK_F15;
	if (!_stricmp("VK_F15", Value)) return VK_F15;

	if (!_stricmp("F16", Value)) return VK_F16;
	if (!_stricmp("VK_F16", Value)) return VK_F16;

	if (!_stricmp("F17", Value)) return VK_F17;
	if (!_stricmp("VK_F17", Value)) return VK_F17;

	if (!_stricmp("F18", Value)) return VK_F18;
	if (!_stricmp("VK_F18", Value)) return VK_F18;

	if (!_stricmp("F19", Value)) return VK_F19;
	if (!_stricmp("VK_F19", Value)) return VK_F19;

	if (!_stricmp("F20", Value)) return VK_F20;
	if (!_stricmp("VK_F20", Value)) return VK_F20;

	if (!_stricmp("F21", Value)) return VK_F21;
	if (!_stricmp("VK_F21", Value)) return VK_F21;

	if (!_stricmp("F22", Value)) return VK_F22;
	if (!_stricmp("VK_F22", Value)) return VK_F22;

	if (!_stricmp("F23", Value)) return VK_F23;
	if (!_stricmp("VK_F23", Value)) return VK_F23;

	if (!_stricmp("F24", Value)) return VK_F24;
	if (!_stricmp("VK_F24", Value)) return VK_F24;

	if (!_stricmp("HOME", Value)) return VK_HOME;
	if (!_stricmp("VK_HOME", Value)) return VK_HOME;

	if (!_stricmp("DELETE", Value)) return VK_DELETE;
	if (!_stricmp("VK_DELETE", Value)) return VK_DELETE;

	if (!_stricmp("PAGEDOWN", Value)) return VK_NEXT;
	if (!_stricmp("VK_PAGEDOWN", Value)) return VK_NEXT;

	if (!_stricmp("PAGEUP", Value)) return VK_PRIOR;
	if (!_stricmp("VK_PAGEUP", Value)) return VK_PRIOR;

	if (!_stricmp("INSERT", Value)) return VK_INSERT;
	if (!_stricmp("VK_INSERT", Value)) return VK_INSERT;

	if (!_stricmp("END", Value)) return VK_END;
	if (!_stricmp("VK_END", Value)) return VK_END;

	if (!_stricmp("TAB", Value)) return VK_TAB;
	if (!_stricmp("VK_TAB", Value)) return VK_TAB;

	if (!_stricmp("CAPSLOCK", Value)) return VK_CAPITAL;
	if (!_stricmp("VK_CAPSLOCK", Value)) return VK_CAPITAL;

	if (!_stricmp("SHIFT", Value)) return VK_SHIFT;
	if (!_stricmp("VK_SHIFT", Value)) return VK_SHIFT;

	if (!_stricmp("ENTER", Value)) return VK_RETURN;
	if (!_stricmp("VK_ENTER", Value)) return VK_RETURN;
	if (!_stricmp("RETURN", Value)) return VK_RETURN;
	if (!_stricmp("VK_RETURN", Value)) return VK_RETURN;

	if (!_stricmp("VK_CTRL", Value)) return VK_CONTROL;
	if (!_stricmp("CONTROL", Value)) return VK_CONTROL;
	if (!_stricmp("VK_CTRL", Value)) return VK_CONTROL;
	if (!_stricmp("VK_CONTROL", Value)) return VK_CONTROL;

	if (!_stricmp("ESC", Value)) return VK_ESCAPE;
	if (!_stricmp("ESCAPE", Value)) return VK_ESCAPE;
	if (!_stricmp("VK_ESC", Value)) return VK_ESCAPE;
	if (!_stricmp("VK_ESCAPE", Value)) return VK_ESCAPE;

	if (!_stricmp("SPACE", Value)) return VK_SPACE;
	if (!_stricmp("VK_SPACE", Value)) return VK_SPACE;

	if (!_stricmp("LEFT", Value)) return VK_LEFT;
	if (!_stricmp("VK_LEFT", Value)) return VK_LEFT;

	if (!_stricmp("UP", Value)) return VK_UP;
	if (!_stricmp("VK_UP", Value)) return VK_UP;

	if (!_stricmp("RIGHT", Value)) return VK_RIGHT;
	if (!_stricmp("VK_RIGHT", Value)) return VK_RIGHT;

	if (!_stricmp("DOWN", Value)) return VK_DOWN;
	if (!_stricmp("VK_DOWN", Value)) return VK_DOWN;

	if (!_stricmp("VK_NUMPAD*", Value)) return VK_MULTIPLY;
	if (!_stricmp("VK_MULTIPLY", Value)) return VK_MULTIPLY;

	if (!_stricmp("VK_NUMPAD+", Value)) return VK_ADD;
	if (!_stricmp("VK_ADD", Value)) return VK_ADD;

	if (!_stricmp("VK_NUMPAD-", Value)) return VK_SUBTRACT;
	if (!_stricmp("VK_SUBTRACT", Value)) return VK_SUBTRACT;

	if (!_stricmp("VK_NUMPAD.", Value)) return VK_DECIMAL;
	if (!_stricmp("VK_DECIMAL", Value)) return VK_DECIMAL;

	if (!_stricmp("VK_NUMPAD/", Value)) return VK_DIVIDE;
	if (!_stricmp("VK_DIVIDE", Value)) return VK_DIVIDE;

	if (!_stricmp("ALT", Value)) return VK_MENU;
	if (!_stricmp("VK_ALT", Value)) return VK_MENU;

	if (!_stricmp("VK_LMENU", Value)) return VK_LMENU;
	if (!_stricmp("VK_RMENU", Value)) return VK_RMENU;

	if (!_stricmp(",", Value)) return VK_OEM_COMMA;
	if (!_stricmp("VK_,", Value)) return VK_OEM_COMMA;
	if (!_stricmp("VK_COMMA", Value)) return VK_OEM_COMMA;

	if (!_stricmp("+", Value)) return VK_OEM_PLUS;
	if (!_stricmp("=", Value)) return VK_OEM_PLUS;
	if (!_stricmp("VK_+", Value)) return VK_OEM_PLUS;
	if (!_stricmp("VK_=", Value)) return VK_OEM_PLUS;
	if (!_stricmp("VK_PLUS", Value)) return VK_OEM_PLUS;

	if (!_stricmp("_", Value)) return VK_OEM_MINUS;
	if (!_stricmp("-", Value)) return VK_OEM_MINUS;
	if (!_stricmp("VK__", Value)) return VK_OEM_MINUS;
	if (!_stricmp("VK_-", Value)) return VK_OEM_MINUS;
	if (!_stricmp("VK_MINUS", Value)) return VK_OEM_MINUS;

	if (!_stricmp(">", Value)) return VK_OEM_PERIOD;
	if (!_stricmp(".", Value)) return VK_OEM_PERIOD;
	if (!_stricmp("VK_>", Value)) return VK_OEM_PERIOD;
	if (!_stricmp("VK_.", Value)) return VK_OEM_PERIOD;
	if (!_stricmp("VK_PERIOD", Value)) return VK_OEM_PERIOD;

	if (!_stricmp("""", Value)) return VK_OEM_7;
	if (!_stricmp("'", Value)) return VK_OEM_7;
	if (!_stricmp("VK_""", Value)) return VK_OEM_7;
	if (!_stricmp("VK_'", Value)) return VK_OEM_7;
	if (!_stricmp("VK_OEM_7", Value)) return VK_OEM_7;

	if (!_stricmp("}", Value)) return VK_OEM_6;
	if (!_stricmp("]", Value)) return VK_OEM_6;
	if (!_stricmp("VK_}", Value)) return VK_OEM_6;
	if (!_stricmp("VK_]", Value)) return VK_OEM_6;
	if (!_stricmp("VK_OEM_6", Value)) return VK_OEM_6;

	if (!_stricmp("|", Value)) return VK_OEM_5;
	if (!_stricmp("\\", Value)) return VK_OEM_5;
	if (!_stricmp("VK_|", Value)) return VK_OEM_5;
	if (!_stricmp("VK_\\", Value)) return VK_OEM_5;
	if (!_stricmp("VK_OEM_5", Value)) return VK_OEM_5;

	if (!_stricmp("{", Value)) return VK_OEM_4;
	if (!_stricmp("[", Value)) return VK_OEM_4;
	if (!_stricmp("VK_{", Value)) return VK_OEM_4;
	if (!_stricmp("VK_[", Value)) return VK_OEM_4;
	if (!_stricmp("VK_OEM_4", Value)) return VK_OEM_4;

	if (!_stricmp("~", Value)) return VK_OEM_3;
	if (!_stricmp("`", Value)) return VK_OEM_3;
	if (!_stricmp("VK_~", Value)) return VK_OEM_3;
	if (!_stricmp("VK_`", Value)) return VK_OEM_3;
	if (!_stricmp("VK_OEM_3", Value)) return VK_OEM_3;

	if (!_stricmp("?", Value)) return VK_OEM_2;
	if (!_stricmp("/", Value)) return VK_OEM_2;
	if (!_stricmp("VK_?", Value)) return VK_OEM_2;
	if (!_stricmp("VK_/", Value)) return VK_OEM_2;
	if (!_stricmp("VK_OEM_2", Value)) return VK_OEM_2;

	if (!_stricmp(":", Value)) return VK_OEM_1;
	if (!_stricmp(";", Value)) return VK_OEM_1;
	if (!_stricmp("VK_:", Value)) return VK_OEM_1;
	if (!_stricmp("VK_;", Value)) return VK_OEM_1;
	if (!_stricmp("VK_OEM_1", Value)) return VK_OEM_1;

	if (!_stricmp("NUMLOCK", Value)) return VK_NUMLOCK;
	if (!_stricmp("VK_NUMLOCK", Value)) return VK_NUMLOCK;

	if (!_stricmp("PRTSCN", Value)) return VK_SNAPSHOT;
	if (!_stricmp("PRINTSCREEN", Value)) return VK_SNAPSHOT;
	if (!_stricmp("SNAPSHOT", Value)) return VK_SNAPSHOT;
	if (!_stricmp("VK_PRTSCN", Value)) return VK_SNAPSHOT;
	if (!_stricmp("VK_PRINTSCREEN", Value)) return VK_SNAPSHOT;
	if (!_stricmp("VK_SNAPSHOT", Value)) return VK_SNAPSHOT;

	if (!_stricmp("PRINT", Value)) return VK_PRINT;
	if (!_stricmp("PRT", Value)) return VK_PRINT;
	if (!_stricmp("VK_PRINT", Value)) return VK_PRINT;
	if (!_stricmp("VK_PRT", Value)) return VK_PRINT;

	if (!_stricmp("HELP", Value)) return VK_HELP;
	if (!_stricmp("VK_HELP", Value)) return VK_HELP;

	if (!_stricmp("SEPARATOR", Value)) return VK_SEPARATOR;
	if (!_stricmp("VK_SEPARATOR", Value)) return VK_SEPARATOR;

	if (!_stricmp("SCRLK", Value)) return VK_SCROLL;
	if (!_stricmp("SCROLLLOCK", Value)) return VK_SCROLL;
	if (!_stricmp("VK_SCRLK", Value)) return VK_SCROLL;
	if (!_stricmp("VK_SCROLLLOCK", Value)) return VK_SCROLL;

	if (!_stricmp("CANCEL", Value)) return VK_CANCEL;
	if (!_stricmp("CONTROLBREAKPROCESSING", Value)) return VK_CANCEL;
	if (!_stricmp("VK_CANCEL", Value)) return VK_CANCEL;
	if (!_stricmp("VK_CONTROLBREAKPROCESSING", Value)) return VK_CANCEL;

	if (!_stricmp("BACKSPACE", Value)) return VK_BACK;
	if (!_stricmp("BACK", Value)) return VK_BACK;
	if (!_stricmp("VK_BACKSPACE", Value)) return VK_BACK;
	if (!_stricmp("VK_BACK", Value)) return VK_BACK;

	if (!_stricmp("CLEAR", Value)) return VK_CLEAR;
	if (!_stricmp("VK_CLEAR", Value)) return VK_CLEAR;

	if (!_stricmp("PAUSE", Value)) return VK_PAUSE;
	if (!_stricmp("PAUSEBREAK", Value)) return VK_PAUSE;
	if (!_stricmp("VK_PAUSE", Value)) return VK_PAUSE;
	if (!_stricmp("VK_PAUSEBREAK", Value)) return VK_PAUSE;

	if (!_stricmp("LWIN", Value)) return VK_LWIN;
	if (!_stricmp("LEFTWINDOWS", Value)) return VK_LWIN;
	if (!_stricmp("VK_LWIN", Value)) return VK_LWIN;
	if (!_stricmp("VK_LEFTWINDOWS", Value)) return VK_LWIN;

	if (!_stricmp("RWIN", Value)) return VK_RWIN;
	if (!_stricmp("RIGHTWINDOWS", Value)) return VK_RWIN;
	if (!_stricmp("VK_RWIN", Value)) return VK_RWIN;
	if (!_stricmp("VK_RIGHTWINDOWS", Value)) return VK_RWIN;

	if (!_stricmp("APPS", Value)) return VK_APPS;
	if (!_stricmp("APPLICATIONS", Value)) return VK_APPS;
	if (!_stricmp("VK_APPS", Value)) return VK_APPS;
	if (!_stricmp("VK_APPLICATIONS", Value)) return VK_APPS;

	if (!_stricmp("SLEEP", Value)) return VK_SLEEP;
	if (!_stricmp("VK_SLEEP", Value)) return VK_SLEEP;

	if (!_stricmp("LSHIFT", Value)) return VK_LSHIFT;
	if (!_stricmp("VK_LSHIFT", Value)) return VK_LSHIFT;

	if (!_stricmp("RSHIFT", Value)) return VK_RSHIFT;
	if (!_stricmp("VK_RSHIFT", Value)) return VK_RSHIFT;

	if (!_stricmp("LCONTROL", Value)) return VK_LCONTROL;
	if (!_stricmp("VK_LCONTROL", Value)) return VK_LCONTROL;

	if (!_stricmp("RCONTROL", Value)) return VK_RCONTROL;
	if (!_stricmp("VK_RCONTROL", Value)) return VK_RCONTROL;

	if (!_stricmp("LMENU", Value)) return VK_LMENU;
	if (!_stricmp("VK_LMENU", Value)) return VK_LMENU;

	if (!_stricmp("RMENU", Value)) return VK_RMENU;
	if (!_stricmp("VK_RMENU", Value)) return VK_RMENU;

	if (!_stricmp("BRIGHT WHITE", Value)) return -1;
	if (!_stricmp("WHITE", Value)) return 0;
	if (!_stricmp("RED", Value)) return 1;
	if (!_stricmp("GREEN", Value)) return 2;
	if (!_stricmp("BLUE", Value)) return 3;
	if (!_stricmp("GOLD", Value)) return 4;
	if (!_stricmp("GRAY", Value)) return 5;
	if (!_stricmp("BLACK", Value)) return 6;
	if (!_stricmp("GOLD 2", Value)) return 7;
	if (!_stricmp("ORANGE", Value)) return 8;
	if (!_stricmp("YELLOW", Value)) return 9;
	if (!_stricmp("DARK GREEN", Value)) return 10;
	if (!_stricmp("PURPLE", Value)) return 11;
	if (!_stricmp("DARK GREEN 2", Value)) return 12;
	if (!_stricmp("BRIGHT WHITE 2", Value)) return 13;
	if (!_stricmp("BLACK 2", Value)) return 14;
	if (!_stricmp("SILVER", Value)) return 15;
	if (!_stricmp("LIGHT GRAY", Value)) return 16;
	if (!_stricmp("MESSY", Value)) return 17;
	if (!_stricmp("BRIGHT WHITE 3", Value)) return 18;
	if (!_stricmp("RED 2", Value)) return 19;

	if (!_stricmp("INFINITE", Value)) return INFINITE;

	return atoi(Value);
}

void Settings::ParseBlind(string Value)
{
	std::stringstream stringStream(Value);
	std::string line;
	vector<string> blindVector;
	while (std::getline(stringStream, line))
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of("},", prev)) != std::string::npos)
		{
			if (pos > prev)
				blindVector.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line.length())
			blindVector.push_back(line.substr(prev, std::string::npos));
	}

	for (auto i : blindVector)
	{
		i.erase(std::remove(i.begin(), i.end(), '{'), i.end());
		std::string input = std::string(i);
		std::istringstream ss(input);
		std::string token;
		while (std::getline(ss, token, ','))
		{
			if (Blinds[0].size() == Blinds[1].size())
				Blinds[0].push_back(stoi(token));
			else Blinds[1].push_back(stoi(token));
		}
	}
}

void Settings::SetBoolValues(LPSTR Value, bool Variable[])
{
	stringstream ss(Value);

	string word;

	int i = 0;

	while (getline(ss, word, ','))
	{
		word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
		Variable[i] = Settings::Bool((LPSTR)word.c_str());
		if (ss.peek() == ',')
			ss.ignore();

		i++;
	}
}

void Settings::SetKeyValues(LPSTR Value, INT Variable[])
{
	stringstream ss(Value);

	string word;

	int i = 0;

	while (getline(ss, word, ','))
	{
		word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());

		if (CheckString((LPSTR)word.c_str()))
			Variable[i] = stoi(word);
		else
			Variable[i] = Settings::Integer((LPSTR)word.c_str());

		if (ss.peek() == ',')
			ss.ignore();

		i++;
	}
}

bool Settings::CheckString(LPSTR Value)
{
	for (int i = 0; i < 255; i++)
	{
		if (!_stricmp(Value, to_string(i).c_str()))
			return true;
	}

	return false;
}

//Converts int to string
string Settings::to_string(INT t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

//SetCurseValue
void Settings::SetCurseValues(LPSTR Value, bool Variable[])
{
	stringstream ss(Value);

	string word;

	int i = 0;

	while (getline(ss, word, ','))
	{
		word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
		Settings::Curses((LPSTR)word.c_str());
		if (ss.peek() == ',')
			ss.ignore();

		i++;
	}
}

//Parses Curses
bool Settings::Curses(LPSTR Value)
{
	CharLower(Value);
	//CURSED
	if (!_stricmp(Value, "[AMPLIFY]"))
		RescueWhenCursed[0] = true;
	if (!_stricmp(Value, "[DIMVISION]"))
		RescueWhenCursed[1] = true;
	if (!_stricmp(Value, "[WEAKEN]"))
		RescueWhenCursed[2] = true;
	if (!_stricmp(Value, "[IRONMAIDEN]"))
		RescueWhenCursed[3] = true;
	if (!_stricmp(Value, "[TERROR]"))
		RescueWhenCursed[4] = true;
	if (!_stricmp(Value, "[CONFUSE]"))
		RescueWhenCursed[5] = true;
	if (!_stricmp(Value, "[LIFETAP]"))
		RescueWhenCursed[6] = true;
	if (!_stricmp(Value, "[ATTRACT]"))
		RescueWhenCursed[7] = true;
	if (!_stricmp(Value, "[DECREPIFY]"))
		RescueWhenCursed[8] = true;
	if (!_stricmp(Value, "[LOWERRESIST]"))
		RescueWhenCursed[9] = true;
	//OUT
	if (!_stricmp(Value, "[BATTLEORDER]"))
		RescueWhenOut[0] = true;
	if (!_stricmp(Value, "[BATTLECOMMAND]"))
		RescueWhenOut[1] = true;
	if (!_stricmp(Value, "[SHOUT]"))
		RescueWhenOut[2] = true;
	if (!_stricmp(Value, "[BONEARMOR]"))
		RescueWhenOut[3] = true;
	if (!_stricmp(Value, "[CYCLONE]"))
		RescueWhenOut[4] = true;
	if (!_stricmp(Value, "[FADE]"))
		RescueWhenOut[5] = true;
	if (!_stricmp(Value, "[HOLYSHIELD]"))
		RescueWhenOut[6] = true;
	if (!_stricmp(Value, "[OAKSAGE]"))
		RescueWhenOut[7] = true;
	if (!_stricmp(Value, "[CHILLINGARMOR]"))
		RescueWhenOut[8] = true;
	if (!_stricmp(Value, "[FROZENARMOR]"))
		RescueWhenOut[9] = true;
	if (!_stricmp(Value, "[ENERGYSHIELD]"))
		RescueWhenOut[10] = true;
	if (!_stricmp(Value, "[ENCHANT]"))
		RescueWhenOut[11] = true;
	//WHEN
	if (!_stricmp(Value, "[HOSTILE]"))
		RescueWhen[0] = true;
	if (!_stricmp(Value, "[FARCAST]"))
		RescueWhen[1] = true;
	if (!_stricmp(Value, "[DIRECT]"))
		RescueWhen[2] = true;
	if (!_stricmp(Value, "[MERCDEATH]"))
		RescueWhen[3] = true;
	if (!_stricmp(Value, "[MERCPOKE]"))
		RescueWhen[4] = true;
	if (!_stricmp(Value, "[MERCPOKENOOAK]"))
		RescueWhen[5] = true;

	return false;
}