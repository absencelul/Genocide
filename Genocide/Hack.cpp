#define _DEFINE_PTRS
#define VARS
#include "Hack.h"
#include "Pointer.h"

#define LeftClickAllSkills    0xEB

Patch * patches[] = {
	new Patch(Call, 0x96DED, (int)Stubs::ChickenSTUB, 5),
	new Patch(Jmp, 0x5ADB3, (int)Stubs::DrawAutomapSTUB, 5),
	new Patch(Jmp, 0x572D8, (int)Stubs::DrawInterfaceSTUB, 6),
	new Patch(Call, 0x4EF28, (int)Stubs::GameFailToJoinSTUB, 6),
	new Patch(Call, 0x5F802, (int)Stubs::GamePacketReceived_STUB, 5),
	new Patch(Call, 0x25A1B3, (int)Stubs::OnGameEndSTUB, 5),
	new Patch(Call, 0x4F28B, (int)Stubs::OnGameLoopSTUB, 6),
	new Patch(Call, 0x6A847, (int)Stubs::OnGameKeyDownSTUB, 7),
	new Patch(Call, 0x418B2, (int)Stubs::RefreshGameListSTUB, 5),
	new Patch(Call, 0x56FAA, (int)Stubs::KeyHookSTUB, 5),
	new Patch(Call, 0xc6926, (int)TMC::GameAfterAttack, 5),
	new Patch(Call, 0x7C89D, (int)Stubs::InputSTUB, 5),
	//new Patch(Call, 0x61704, (DWORD)GameAttack_Intercept, 5),
	new Patch(Call, 0x7B8CD, (int)Stubs::ChatText, 5),
	new Patch(Call, 0x7B472, (int)Stubs::ChatText, 5),
	new Patch(Call, 0x596d9, (int)AutomapCells, 5),
	new Patch(Call, 0x78525, (int)OnGamePacketSent, 5),
	new Patch(Fill, 0x08862, (int)INST_NOP, 17),
	new Patch(Fill, 0x085F7, (int)INST_NOP, 17),
	new Patch(Fill, 0xAA490, LeftClickAllSkills, 1),

	/*
	{ PatchCall, GetOffset(0x96DED), (DWORD)Stubs::ChickenSTUB, 5 }, // updated 1.14d
	{ PatchJmp, GetOffset(0x5ADB3), (DWORD)Stubs::DrawAutomapSTUB, 5 }, // updated 1.14d
	{ PatchJmp, GetOffset(0x572D8), (DWORD)Stubs::DrawInterfaceSTUB, 6 }, //upodated 1.14d
	{ PatchCall, GetOffset(0x4EF28),  (DWORD)Stubs::GameFailToJoinSTUB, 6 }, //Updated 1.14d
	{ PatchCall, GetOffset(0x5F802), (DWORD)Stubs::GamePacketReceived_STUB, 5 }, // updated 1.14d
	{ PatchCall, GetOffset(0x25A1B3), (DWORD)Stubs::OnGameEndSTUB, 5 }, //updated 1.14d
	{ PatchCall, GetOffset(0x4F28B), (DWORD)Stubs::OnGameLoopSTUB, 6 }, // updated 1.14d
	{ PatchCall, GetOffset(0x6A847), (DWORD)Stubs::OnGameKeyDownSTUB, 7 }, //updated 1.14d
	{ PatchCall, GetOffset(0x418B2), (DWORD)Stubs::RefreshGameListSTUB, 5 }, //updated 1.14d
	{ PatchCall, GetOffset(0x56FAA), (DWORD)Stubs::KeyHookSTUB, 5 }, //updated 1.14d
	{ PatchCall, GetOffset(0xc6926), (DWORD)TMC::GameAfterAttack, 5 }, //updated 1.14d
	{ PatchCall, GetOffset(0x7C89D), (DWORD)Stubs::InputSTUB, 5 }, //updated 1.14d
	//{ PatchCall, GetOffset(0x61704), (DWORD)GameAttack_Intercept, 5 }, //Updated 1.14d
	{ PatchCall, GetOffset(0x7B8CD), (DWORD)Stubs::ChatText,5 }, //Chat Message
	{ PatchCall, GetOffset(0x7B472), (DWORD)Stubs::ChatText,5 }, //Chat _
	{ PatchCall, GetOffset(0x596d9), (DWORD)AutomapCells, 5 }, //Updated 1.14d
	{ PatchCall, GetOffset(0x78525), (DWORD)OnGamePacketSent, 5 }, //Updated 1.14d
	{ PatchFill, GetOffset(0x08862), (DWORD)INST_NOP, 17 }, // Block Error Report
	{ PatchFill, GetOffset(0x085F7), (DWORD)INST_NOP, 17 }, // Block Error Report
	*/
};

bool Inject::Inject(HMODULE hMod, LPVOID lpReserved)
{
	_dll = hMod;
	Settings::LoadConfig();
	Settings::Config();
	ReadConfig("Spam Filter.txt", &SpamMsg);
	ReadConfig("Spam Players.txt", &SpamPlayers);

	if (D2GFX_GetHwnd())
	{
		OldWndProc = (WNDPROC)GetWindowLongA(D2GFX_GetHwnd(), GWL_WNDPROC);
		SetWindowLongA(D2GFX_GetHwnd(), GWL_WNDPROC, (LONG)KeyHooks::WindowProc);
	}

	for (int i = 0; i < (sizeof(patches) / sizeof(Patch*)); i++)
		patches[i]->Install();

	InitializeClass(0);
	return true;
}

bool Inject::Uninject()
{
	if (D2GFX_GetHwnd())
		SetWindowLongA(D2GFX_GetHwnd(), GWL_WNDPROC, (LONG)OldWndProc);

	for (int i = 0; i < (sizeof(patches) / sizeof(Patch*)); i++)
		patches[i]->Uninstall();

	InitializeClass(1);
	return true;
}

void Inject::InitializeClass(int Initialize)
{
	switch (Initialize)
	{
	case 0:
		Revealz = new Reveal;
		//Items = new MoveItems;
		break;

	case 1:
		delete Revealz;
		//delete Items;
		break;
	}
}