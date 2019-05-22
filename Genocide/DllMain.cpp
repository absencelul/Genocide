#include "Hack.h"

bool APIENTRY DllMain(HMODULE hMod, DWORD reason, LPVOID lpReserved)
{
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);

		if (GetModuleHandleA("Game.exe"))
			return Inject::Inject(hMod, lpReserved);
		break;
	case DLL_PROCESS_DETACH:
		return Inject::Uninject();
		break;
	}

	return true;
}