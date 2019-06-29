#include "Hack.h"
static HANDLE PH = 0;

KeyHooks::KeyHooks()
{
}

KeyHooks::~KeyHooks()
{
}

Patch* invPatches[] = {
	new Patch(Call, 0x845D0, (DWORD)Stubs::ViewInventorySTUB1, 5),
	new Patch(Call, 0x873DB, (DWORD)Stubs::ViewInventorySTUB2, 5),
	new Patch(Call, 0x8E241, (DWORD)Stubs::ViewInventorySTUB3, 6),
};

DWORD WINAPI KeyHooks::PrecastThread(LPVOID Parameter)
{
	if (!*p_D2CLIENT_isWeaponSwitch)
		SwitchWeapon();

	while (!*p_D2CLIENT_isWeaponSwitch)
		SleepEx(1, true);

	PrecastSkills cast[] =
	{
		{ D2S_BATTLECOMMAND, Funcs->GetSkill(D2S_BATTLECOMMAND), Funcs->GetCurSkill(false, D2S_BATTLECOMMAND) },
		{ D2S_BATTLEORDERS,  Funcs->GetSkill(D2S_BATTLEORDERS), Funcs->GetCurSkill(false, D2S_BATTLEORDERS) },
		{ D2S_HOLYSHIELD, Funcs->GetSkill(D2S_HOLYSHIELD), Funcs->GetCurSkill(false, D2S_HOLYSHIELD) },
		{ D2S_BATTLECOMMAND, Funcs->GetSkill(D2S_BATTLECOMMAND), Funcs->GetCurSkill(false, D2S_BATTLECOMMAND) },
	};

	for (int i = 0; i < ArraySize(cast); i++)
	{
		if (cast[i].GetSkill)
		{
			if (!Funcs->GetCurSkill(false, cast[i].Skill))
				SetSkill(cast[i].Skill, false);

			//Funcs->Print(false, 4, "%d", Funcs->GetCurrentSkill(false));

			while (!Funcs->GetCurSkill(false, cast[i].Skill))
				SleepEx(1, true);

			SleepEx(150, true);

			AttackStruct Attack;

			Attack.dwAttackType = 0x66;

			Attack.dwTargetX = Me->pPath->xPos;
			Attack.dwTargetY = Me->pPath->yPos;

			Attack.lpPlayerUnit = Me;

			Attack.lpTargetUnit = NULL;

			Stubs::D2CLIENT_Attack(&Attack, false);

			SleepEx(150, true);
		}
	}

	SleepEx(400, true);

	if (*p_D2CLIENT_isWeaponSwitch)
		SwitchWeapon();

	while (*p_D2CLIENT_isWeaponSwitch)
		SleepEx(1, true);

	PH = 0;

	return 0;
}

void KeyHooks::KeyHook()
{
	if (KeyDown(hammer[0]))
	{
		if (Funcs->GetCurrentSkill(false) != D2S_CONCENTRATION)
			SetSkill(D2S_CONCENTRATION, false);

		if (Funcs->GetCurrentSkill(true) != D2S_BLESSEDHAMMER)
			SetSkill(D2S_BLESSEDHAMMER, true);

		WORD x = (WORD)Units->GetUnitLocation(true).x, y = (WORD)Units->GetUnitLocation(true).y;
		POINT Position = { (long)* p_D2CLIENT_MouseX, (long)* p_D2CLIENT_MouseY };

		Funcs->ScreenToAutoMap(&Position);

		switch (hammer[2])
		{
		case 0:
			if (x != 0 && x - Me->pPath->xPos < 40 && y != 0 && y - Me->pPath->yPos < 40)
				return Funcs->CastSpell(x, y, false);
			else
				return Funcs->CastSpell((WORD)Position.x, (WORD)Position.y, false);
			break;

		case 1:
			if (x != 0 && x - Me->pPath->xPos == 2 && y != 0 && y - Me->pPath->yPos == -1)
				return Funcs->CastSpell(2, -2, true);
			else if ((x != 0 || x != NULL) && x - Me->pPath->xPos < 40 && (y != 0 || y != NULL) && y - Me->pPath->yPos < 40)
				return Funcs->CastSpell(x, y, false);
			else
				return Funcs->CastSpell(-3, 0, true);
			break;

		case 2:

			return Funcs->CastSpell((WORD)Position.x, (WORD)Position.y, false);

			break;
		}
	}

	if (KeyDown(AutoAim[0]))
	{
		switch (AutoAim[3])
		{
		case 0:
			return AttackNearest(false);
			break;

		case 1:
			if (Units->FindUnit())
				return AttackTarget(false);
			break;
		}
	}

	if (KeyDown(AutoAim[1]))
	{
		switch (AutoAim[3])
		{
		case 0:
			return AttackNearest(true);
			break;

		case 1:
			if (Units->FindUnit())
				return AttackTarget(true);
			break;
		}
	}

	if (KeyDown(OneKey))
	{
		TestKey();
	}
}

void __fastcall KeyHooks::OnGameKeyDown(byte Key, byte Repeat)
{
	//MoveItems* Items = new MoveItems();
	//qw32r4k3nn0zhej // 0803 // Paladin
	//pKtvdUmK0HYU6ik // 0803 // Amazon
	//y5t4u3yCJnEngJD // 0803 // Necro
	if (Key == Blindz[1])
	{
		if ((unsigned)Blindz[0] < Blinds[0].size() - 1)
			Blindz[0]++;
		else
			Blindz[0] = 0;

		Funcs->InputConsole("Setting Blindspot - (ÿc0%dÿc4) [ÿc0%dÿc4,ÿc0%dÿc4]", Blindz[0], Blinds[0].at(Blindz[0]), Blinds[1].at(Blindz[0]));
		//Funcs->Print(0, 4, "Setting Blindspot - (ÿc0%dÿc4) [ÿc0%dÿc4,ÿc0%dÿc4]", Blindz[0], Blinds[0].at(Blindz[0]), Blinds[1].at(Blindz[0]));
		//Items->MoveBufferToBuffer(STORAGE_STASH, STORAGE_INVENTORY);
	}

	/*if (Key == StashCube[0])
	{
		Items->MoveBufferToBuffer(STORAGE_INVENTORY, STORAGE_STASH);
	}

	if (Key == StashCube[1])
	{
		Items->MoveBufferToBuffer(STORAGE_INVENTORY, 255);
	}*/

	if (Key == StashCube[0])
	{
		if (p_D2CLIENT_UIMode[UI_CUBE])
			D2CLIENT_SetUIVar(UI_CUBE, 1, 0);
		if (p_D2CLIENT_UIMode[UI_STASH])
			D2CLIENT_SetUIVar(UI_STASH, 1, 0);
		else
		{
			BYTE data[2] = { 0x77, 0x10 };
			D2NET_ReceivePacket(data, 2);
		}
	}

	if (Key == StashCube[1])
	{
		if (p_D2CLIENT_UIMode[UI_STASH])
			D2CLIENT_SetUIVar(UI_STASH, 1, 0);
		if (p_D2CLIENT_UIMode[UI_CUBE])
			D2CLIENT_SetUIVar(UI_CUBE, 1, 0);
		else
		{
			BYTE data[2] = { 0x77, 0x15 };
			D2NET_ReceivePacket(data, 2);
		}
	}

	//town
	if (Key == TownKeys[1])
	{
		if (Portal())
			Town = true;
	}
	//opentp
	if (Key == TownKeys[0])
	{
		Portal();
	}

	if (Key == TeleWalk)
	{
		POINT Position = { (long)* p_D2CLIENT_MouseX, (long)* p_D2CLIENT_MouseY };

		Funcs->ScreenToAutoMap(&Position);

		Teleportz((WORD)Position.x, (WORD)Position.y);

		if (D2CLIENT_GetSelectedUnit())
			Interact(D2CLIENT_GetSelectedUnit()->dwUnitId, D2CLIENT_GetSelectedUnit()->dwType);
	}

	if (Key == TeleWalkHostile)
	{
		if (Players.IsEmpty())
			return;

		UnitAny* pMe = D2CLIENT_GetPlayerUnit();

		for (RosterUnit* pRoster = *p_D2CLIENT_PlayerUnitList; pRoster; pRoster = pRoster->pNext)
		{
			if (pRoster->dwUnitId == pMe->dwUnitId)
				continue;

			UnitAny* pTarget = D2CLIENT_FindServerSideUnit(pRoster->dwUnitId, UNIT_TYPE_PLAYER);

			if (!pTarget)
				continue;

			if (IsTown(pMe))
				if (Units->GetRelation(pTarget) != 4)
					HostilePlayer(pTarget->dwUnitId);
		}

		/*LPUNITANY Target = (LPUNITANY)GetUnit(Players[CurrentTarget]->UnitId, UNIT_TYPE_PLAYER);

		if (Players.GetSize() != NULL && Target)
		{
			if (IsTown(Me) && Target)
			{
				HostilePlayer(Players[CurrentTarget]->UnitId);
				Teleportz(Target->pPath->xPos + Blinds[0].at(Blindz[0]), Target->pPath->yPos + Blinds[1].at(Blindz[0]));
			}
		}*/
	}

	if (Key == Prebuff)
	{
		if (!PH)
			PH = (HANDLE)CreateThread(0, 0, PrecastThread, 0, 0, 0);
	}

	if (Key == ViewPlayerGear)
	{
		if (Players.IsEmpty())
			return;
		else
			for (int i = 0; i < (sizeof(invPatches) / sizeof(Patch*)); i++)
				invPatches[i]->Install();

		if (!p_D2CLIENT_UIMode[UI_CHAT])
		{
			LPUNITANY Unit = D2CLIENT_GetSelectedUnit();

			if (!Unit)
				Unit = (LPUNITANY)GetUnit(Players[CurrentTarget]->UnitId, UNIT_TYPE_PLAYER);

			if (Unit && Unit->pInventory && Unit->dwType == UNIT_TYPE_PLAYER)
			{
				if (Unit->dwMode != PLAYER_MODE_DEAD && Unit->dwMode != PLAYER_MODE_DEATH)
					ViewingUnit = Unit;

				Funcs->Print(0, 4, "Inspecting inventory of [%s]", Unit->pPlayerData->szName);

				if (!p_D2CLIENT_UIMode[UI_INVENTORY])
					D2CLIENT_SetUIVar(UI_INVENTORY, 0, 0);
			}
		}
	}

	if (Key == TargetKeys[1])
	{
		NextTar();
	}

	if (Key == TargetKeys[0])
	{
		PrevTar();
	}

	if (Key == Reload)
	{
		Settings::LoadConfig();
		Funcs->InputConsole("Configuration - [Reloaded]");
	}

	if (Key == hammer[1])
	{
		if (hammer[2] < 2)
			hammer[2]++;
		else hammer[2] = 0;

		switch (hammer[2])
		{
		case 0:
			Funcs->InputConsole("Smart Angle is Disabled.");
			break;

		case 1:
			Funcs->InputConsole("Smart Angle is Enabled.");
			break;

		case 2:
			Funcs->InputConsole("Smart Angle is angling at Mouse.");
			break;
		}
	}

	if (Key == AutoAim[2])
	{
		if (AutoAim[3] < 1)
			AutoAim[3]++;
		else AutoAim[3] = 0;

		switch (AutoAim[3])
		{
		case 0:
			Funcs->InputConsole("Switching Aim type to [Nearest Target]");
			break;
		case 1:
			Funcs->InputConsole("Switching Aim type to [Selected Target]");
			break;
		}
	}

	if (Key == AuraFlash)
	{
		Auras = !Auras;
		Funcs->InputConsole(Auras ? "Aura Shifting - [Enabled]." : "Aura Shifting - [Disabled].");
	}
}

//handles hwnd, mouse clicks and location
LONG __stdcall KeyHooks::WindowProc(HWND HWnd, UINT MSG, WPARAM WParam, LPARAM LParam)
{
	switch (MSG)
	{
	case WM_MBUTTONDOWN:

		if (ClientReady)
			FcTele = true;

		break;
	case WM_MBUTTONUP:

		SendMessageA(D2GFX_GetHwnd(), WM_RBUTTONUP, NULL, LOWORD((*p_D2CLIENT_MouseX)) | HIWORD((*p_D2CLIENT_MouseY)));
		break;

		/*case WM_RBUTTONDOWN:
			if (Funcs->GetCurSkill(true, D2S_TELEPORT) && !IsTown(Me))
			{
				SwapMouseButton(true);
				keybd_event(VK_SHIFT, 0, 0, 0);
			}
			break;

		case WM_RBUTTONUP:
			SwapMouseButton(false);
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);*/
	}

	return (LONG)CallWindowProcA(OldWndProc, HWnd, MSG, WParam, LParam);
}