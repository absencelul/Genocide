#include "Hack.h"
static HANDLE EN = 0;

void DrawScreen()
{
	DrawOutConsole();
	DrawOutOrbs();

	if (showStatScreen) extraDraw();

	TimerBo();
	TargetList();

	/* Re-do the Battle Orders Timer*/
	//TextHook(15, 12, -1, 0, 12, 9, BoTimer >= 1 ? "BO: %d" : "", BoTimer);

	if (ScreenShot && TakeScreen && (GetTickCount64() - SStime) >= SSDelay)
	{
		TextHook(30, 585, White, None, 4, "ÿc1%s ÿc;%s", Potions[0] >= 1 ? Settings::to_string(Potions[0]).c_str() : "Off", Potions[1] >= 1 ? Settings::to_string(Potions[1]).c_str() : "Off");
		TextHook(30, 598, White, None, 4, "ÿc8%s ÿc7%s", Chickens[0] >= 1 ? Settings::to_string(Chickens[0]).c_str() : "Off", Chickens[1] >= 1 ? Settings::to_string(Chickens[1]).c_str() : "Off");
		TextHook(796, 585, White, None, 4, "ÿc3%s ÿc;%s", Potions[2] >= 1 ? Settings::to_string(Potions[2]).c_str() : "Off", Potions[3] >= 1 ? Settings::to_string(Potions[3]).c_str() : "Off");
		TextHook(795, 598, White, None, 4, "ÿc8%s ÿc7%s", Chickens[2] >= 1 ? Settings::to_string(Chickens[2]).c_str() : "Off", Chickens[3] >= 1 ? Settings::to_string(Chickens[3]).c_str() : "Off");
		TextHook(375, 15, Gold, None, 4, "%s", Funcs->SysTime());

		TakeScreen = false;
		D2WIN_TakeScreenshot();

		if (UseAltScreen)
			D2CLIENT_SetUIVar(UI_ALTDOWN, 1, 0);
	}

	if (FcTele)
	{
		//if (Funcs->GetCurrentSkill(false) != D2S_TELEPORT)
		if (!Funcs->GetCurSkill(true, D2S_TELEPORT))
			SetSkill(D2S_TELEPORT, true);
		if (!Funcs->GetCurSkill(false, D2S_HOLYFREEZE))
			SetSkill(D2S_HOLYFREEZE, false);

		AttackStruct Attack;
		//Attack.dwAttackType = 0x46;
		Attack.dwAttackType = 0xE5; //shift+left
		bool SelectedSkill = Funcs->GetCurSkill(true, D2S_TELEPORT);
		POINT Target = { (long)(Attack.dwTargetX=0), (long)(Attack.dwTargetY=0) };

		if (!KeyDown(4)) {
			FcTele = false;
		}

		if (Units->CheckReady(true, true) && SelectedSkill)
		{

			POINT Farcast = Math::PickFarcastLocation(Target);
			Attack.dwTargetX = Farcast.x;
			Attack.dwTargetY = Farcast.y;
			Attack.lpPlayerUnit = Me;
			Attack.lpTargetUnit = NULL;
			Stubs::D2CLIENT_Attack(&Attack, false);
		}
	}

	AuraSwap();

	if (!GetUnitState(Units->FindMercUnit(Me), AFFECT_ENCHANT) && Funcs->FindItem(214, STORAGE_EQUIP))
		if (!EN)
			EN = (HANDLE)CreateThread(0, 0, EnchThread, 0, 0, 0);
}

DWORD WINAPI EnchThread(LPVOID Param)
{
	WORD tmpSkill = Funcs->GetCurrentSkill(false);

	if (Units->FindMercUnit(Me) && !GetUnitState(Units->FindMercUnit(Me), AFFECT_ENCHANT) && Funcs->FindItem(214, STORAGE_EQUIP))
	{
		if (Funcs->GetCurrentSkill(false) != D2S_ENCHANT)
			SetChargedSkill(D2S_ENCHANT, false, Me->pInventory->dwLeftItemUid);

		SleepEx(150, true);

		UseSkillOnUnit(Units->FindMercUnit(Me), D2S_ENCHANT, false, false, true);

		SleepEx(150, true);

		SetSkill(tmpSkill, false);
	}
	SleepEx(300, true);
	EN = 0;
	return 0;
}

void extraDraw()
{
	if (p_D2CLIENT_UIMode[UI_CHAT])
	{
		DrawBoxA(125, 545, 675, 498, -1, 6, 2);
	}

	/*if (p_D2CLIENT_UIMode[UI_CHARACTER]) // Draw text to CHAR Menu
	{
		//TextHook(167, 230, 0, 0, );
		TextHook(165, 180, 0, 0, 1, 2, "[%d]", GetUnitStat(Me, STAT_STRENGTH));
		TextHook(165, 240, 0, 0, 1, 2, "[%d]", GetUnitStat(Me, STAT_DEXTERITY));
		TextHook(165, 327, 0, 0, 1, 2, "[%d]", GetUnitStat(Me, STAT_VITALITY));
		TextHook(165, 388, 0, 0, 1, 2, "[%d]", GetUnitStat(Me, STAT_ENERGY));

		TextHook(250, 270, 4, 0, 1, 2, "%d%%", GetUnitStat(Me, STAT_TOBLOCK));
		TextHook(259, 288, 8, 0, 1, 2, "FRW: %d%%", GetUnitStat(Me, STAT_FASTERRUNWALK));
		TextHook(333, 288, 8, 0, 1, 2, "FBR: %d%%", GetUnitStat(Me, STAT_FASTERBLOCK));
		TextHook(259, 350, 8, 0, 1, 2, "DR: %d%%", GetUnitStat(Me, STAT_DAMAGEREDUCTION));
		TextHook(333, 350, 8, 0, 1, 2, "MDR: %d", GetUnitStat(Me, STAT_MAGICDAMAGEREDUCTION));
		//TextHook(245, 333, 4, 0, 1, 2, "%d", GetUnitStat(Me, STAT_MAXHP));
		//TextHook(245, 368, 4, 0, 1, 2, "%d", GetUnitStat(Me, STAT_MAXMANA));

		TextHook(105, 258, 2, 0, 1, 2, "GF: %d%%", GetUnitStat(Me, STAT_GOLDFIND));
		TextHook(105, 275, 2, 0, 1, 2, "MF: %d%%", GetUnitStat(Me, STAT_MAGICFIND));

		TextHook(105, 402, 0, 0, 1, 2, "FCR:  %d%%", GetUnitStat(Me, STAT_FASTERCAST));
		TextHook(105, 413, 0, 0, 1, 2, "IAS:  %d%%", GetUnitStat(Me, STAT_IAS));
		TextHook(105, 424, 0, 0, 1, 2, "FHR:  %d%%", GetUnitStat(Me, STAT_FASTERHITRECOVERY));
		TextHook(105, 435, 0, 0, 1, 2, "DS:   %d%%", GetUnitStat(Me, STAT_DEADLYSTRIKE));
		TextHook(105, 446, 0, 0, 1, 2, "CB:   %d%%", GetUnitStat(Me, STAT_CRUSHINGBLOW));
		TextHook(105, 457, 0, 0, 1, 2, "OW:  %d%%", GetUnitStat(Me, STAT_OPENWOUNDS));
		TextHook(105, 468, 0, 0, 1, 2, "LL:   %d%%", GetUnitStat(Me, STAT_LIFELEECH));
		TextHook(105, 479, 0, 0, 1, 2, "ML:   %d%%", GetUnitStat(Me, STAT_MANALEECH));

		TextHook(256, 407, 1, 0, 1, 2, "%d", GetUnitStat(Me, STAT_FIRERESIST));
		TextHook(376, 407, 1, 0, 8, 2, "%d", GetUnitStat(Me, STAT_MAXFIRERESIST));
		TextHook(255, 390, 1, 0, 13, 2, "%d", GetUnitStat(Me, STAT_FIREABSORB));
		TextHook(270, 390, 1, 0, 13, 2, "/ %d", GetUnitStat(Me, STAT_FIREABSORBPERCENT));

		TextHook(256, 430, 3, 0, 1, 2, "%d", GetUnitStat(Me, STAT_COLDRESIST));
		TextHook(376, 430, 3, 0, 8, 2, "%d", GetUnitStat(Me, STAT_MAXCOLDRESIST));
		TextHook(305, 390, 3, 0, 13, 2, "%d", GetUnitStat(Me, STAT_COLDABSORB));
		TextHook(320, 390, 3, 0, 13, 2, "/ %d", GetUnitStat(Me, STAT_COLDABSORBPERCENT));

		TextHook(256, 455, 9, 0, 1, 2, "%d", GetUnitStat(Me, STAT_LIGHTNINGRESIST));
		TextHook(376, 455, 9, 0, 8, 2, "%d", GetUnitStat(Me, STAT_MAXLIGHTNINGRESIST));
		TextHook(357, 390, 9, 0, 13, 2, "%d", GetUnitStat(Me, STAT_LIGHTNINGABSORB));
		TextHook(372, 390, 9, 0, 13, 2, "/ %d", GetUnitStat(Me, STAT_LIGHTNINGABSORBPERCENT));

		TextHook(256, 480, 10, 0, 1, 2, "%d", GetUnitStat(Me, STAT_POISONRESIST));
		TextHook(376, 480, 10, 0, 8, 2, "%d", GetUnitStat(Me, STAT_MAXPOISONRESIST));
		TextHook(217, 445, 10, 0, 1, 2, "%d%%", GetUnitStat(Me, STAT_POISONLENGTHREDUCTION));

		TextHook(217, 425, 8, 0, 1, 2, "%d", GetUnitStat(Me, STAT_MAGICRESIST));
		TextHook(230, 425, 8, 0, 8, 2, "%d", GetUnitStat(Me, STAT_MAXMAGICRESIST));

	}*/

	if (p_D2CLIENT_UIMode[UI_INVENTORY]) // Draw text to INVENTORY
	{
		TextHook(505, 488, Tan, Center, 1, "Total Gold: %s", Funcs->AddCommas(GetUnitStat(Me, STAT_GOLD) + GetUnitStat(Me, STAT_GOLDBANK), 0));
	}
}

//Draws Target List
void TargetList()
{
	if (!ClientReady)
		return;

	if (!p_D2CLIENT_UIMode[UI_CUBE])
	{
		for (int i = 0; i < Players.GetSize(); i++)
		{
			CHAR PlayerInfo[100];
			sprintf_s(PlayerInfo, "%s %s %d", Players[i]->PlayerName, Units->PlayerClass(Players[i]->ClassId, true), Players[i]->Life);
			DrawTextX(5, 375 + (i * 15), i == CurrentTarget ? 5 : 4, 3, TP->IsTown(Me) ? -1 : 1, "%s", PlayerInfo);
		}
	}
}

//BO Timer
void TimerBo()
{
	if (!ClientReady)
		return;

		if (BoTimer > 0 && (GetTickCount64() - BoTime) >= 1000)
		{
			BoTimer--;
			BoTime = GetTickCount64();
			if (BoTimer == 10)
				Funcs->Print(0, 4, ":: Warning : BO will run out in 10 seconds.");
		}
}

//Changes Orb Colors
unsigned int GetOrbColor(unsigned int nMax)
{
	return (nMax < 30) ? 1 : (nMax < 70) ? 8 : 4;
}

//Draws Orbs
void DrawOutOrbs()
{
	//DrawBoxA(52, 528, 91, 545, 19, 0, 5);
	//DrawBoxA(717, 528, 756, 545, 0x04, 0, 5);
	int xSize[2];
	xSize[0] = GetTextSize(std::to_string(TP->LifeMana(true)), 0).x + 21;
	xSize[1] = GetTextSize(std::to_string(TP->LifeMana(false)), 0).x + 21;
	OnDrawBox(52, xSize[0], 528, 15, 0, BTFull);
	OnDrawBox(717, xSize[1], 528, 15, 0, BTFull);
	TextHook(52 + 4, 528 + 3, White, None, 8, "ÿc%d%d%%", GetOrbColor(TP->LifeMana(true)), TP->LifeMana(true));
	TextHook(717 + 4, 528 + 3, White, None, 8, "ÿc%d%d%%", GetOrbColor(TP->LifeMana(false)), TP->LifeMana(false));
}

//Aura Flash
void AuraSwap()
{
	if (!Auras)
		return;

	else {
		if (AllowSwap)
		{
			AllowSwap = false;

			PrecastSkills swap[] = {
				{ D2S_BLESSEDAIM, Funcs->GetSkill(D2S_BLESSEDAIM) },
				{ D2S_MIGHT, Funcs->GetSkill(D2S_MIGHT) },
				{ D2S_FANATICISM, Funcs->GetSkill(D2S_FANATICISM) },
				{ D2S_CONCENTRATION, Funcs->GetSkill(D2S_CONCENTRATION) },
			};

			for (int i = 0; i < ArraySize(swap); i++)
				if (swap[i].GetSkill)
					SetSkill(swap[i].Skill, false);

			if (TeleSwap)
				SetSkill(D2S_TELEPORT, false);

			TeleSwap = false;
		}
	}
}

void DrawBoxA(int x1, int y1, int x2, int y2, int nLineColor, int nBackGroundColor, int nTrans)
{
	D2GFX_DrawRectangle(x1, y1, x2, y2, nBackGroundColor, nTrans);
	D2GFX_DrawLine(x1, y1, x2, y1, nLineColor, -1);
	D2GFX_DrawLine(x1, y1, x1, y2, nLineColor, -1);
	D2GFX_DrawLine(x2, y1, x2, y2, nLineColor, -1);
	D2GFX_DrawLine(x1, y2, x2, y2, nLineColor, -1);
}

void OnDrawBox(long x, long xSize, long y, long ySize, DWORD dwColor, BoxTrans trans)
{
	RECT pRect = { x, y, x + xSize, y + ySize };
	D2GFX_DrawRectangle(x, y, x + xSize, y + ySize, dwColor, trans);
	Stubs::DrawRectStub(&pRect);
}

POINT GetTextSize(string text, unsigned int font)
{
	unsigned int height[] = { 10,11,18,24,10,13,7,13,10,12,8,8,7,12 };
	DWORD width, fileNo;
	wchar_t* wString = AnsiToUnicode(text.c_str());
	DWORD oldFont = D2WIN_SetTextSize(font);
	D2WIN_GetTextWidthFileNo(wString, &width, &fileNo);
	D2WIN_SetTextSize(oldFont);
	POINT point = { (long)width, (long)height[font] };
	delete[] wString;
	return point;
}

void BoxHook(unsigned int x, unsigned int y, unsigned int xSize, unsigned int ySize, DWORD dwColor, BoxTrans Trans)
{
	return D2GFX_DrawRectangle(x, y, x + xSize, y + ySize, dwColor, Trans);
}

//Draws text to screen
void TextHook(int x, int y, TextColor color, Alignment align, unsigned int font, std::string text, ...)
{
	char szBuffer[4096];
	va_list args;
	va_start(args, text);
	vsprintf_s(szBuffer, 4096, text.c_str(), args);
	va_end(args);

	// Convert multibyte to widechar
	wchar_t* wString = AnsiToUnicode(szBuffer);

	// Alignment
	switch (align)
	{
	case Center:
		x = x - (GetTextSize(szBuffer, font).x / 2);
		break;

	case Right:
		x = x - GetTextSize(szBuffer, font).x;
		break;
	}

	unsigned int height[] = { 10,11,18,24,10,13,7,13,10,12,8,8,7,12 };
	DWORD size = D2WIN_SetTextSize(font);
	D2WIN_DrawText(wString, x, y + height[font], color, 0);
	D2WIN_SetTextSize(size);

	delete[] wString;
}

void DrawTextX(int X, int Y, int Color, int Cent, int TransLvl, char* Msg, ...)
{
	va_list arguments;
	va_start(arguments, Msg);

	int len = _vscprintf(Msg, arguments) + 1;
	char * text = new char[len];
	vsprintf_s(text, len, Msg, arguments);
	va_end(arguments);

	wchar_t* wtext = new wchar_t[len];
	CharToWide(text, len, wtext, len);

	D2WIN_DrawTextEx(wtext, X, Y, Color, Cent, TransLvl);

	delete[] text;
	delete[] wtext;
}

void CharToWide(char* lpMultiByteStr, int cbMultiByte, wchar_t* lpWideCharStr, int cchWideChar)
{
	MultiByteToWideChar(0, 1, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
	for (DWORD i = 0; i < wcslen(lpWideCharStr); i++)
	{
		if (lpWideCharStr[i] >= 0xFF && lpWideCharStr[i + 1] == L'c')
		{
			if (lpWideCharStr[i + 2] >= L'0' && lpWideCharStr[i + 2] <= L'9')
			{
				lpWideCharStr[i] = 0xFF;
			}
		}

	}
}

void MyMultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, INT cbMultiByte, LPWSTR lpWideCharStr, INT cchWideChar)
{
	MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);

	for (DWORD i = 0; i < wcslen(lpWideCharStr); i++)
	{
		if (lpWideCharStr[i] >= 0xFF && lpWideCharStr[i + 1] == L'c')
		{
			if (lpWideCharStr[i + 2] >= L'0' && lpWideCharStr[i + 2] <= L'9')
			{
				lpWideCharStr[i] = 0xFF;
			}
		}
	}
}

INT TextWidth(LPSTR szText)
{
	WCHAR wText[66666] = { 0 };

	MyMultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szText, (INT)strlen(szText), wText, (INT)strlen(szText));

	Funcs->RemoveColorSpecs(wText);

	return D2WIN_GetTextWidth(wText);
}

void DrawOutConsole()
{
	std::vector<std::string> TempConsole;

	for (unsigned int i = 0; i < ConsoleMessages.size() && i < 3; ++i)
		if (GetTickCount64() - ConsoleMessages[ConsoleMessages.size() - (i + 1)]->dwTimer < ConsoleDelay)
			TempConsole.push_back(ConsoleMessages[ConsoleMessages.size() - (i + 1)]->ConsoleString);

	if (TempConsole.size())
	{
		long xSize = 0;
		for (unsigned int i = 0; i < ConsoleMessages.size(); i++)
			xSize = GetTextSize(ConsoleMessages.at(i).str(), 13).x + 21;
				//size = TextWidth((LPSTR)cMessages.at(b).c_str(), 4);
		OnDrawBox(185, xSize, 520 - (12 * (TempConsole.size() - 1)), 20, Gold, BTFull);
		DrawBoxA(185, 520 - (12 * (TempConsole.size() - 1)), 610, 540, 19, 0, 2);

		for (unsigned int i = 0; i < TempConsole.size(); ++i)
		{
			TextHook(190, 537 - (i * 12), Grey, None, 13, (LPSTR)TempConsole[i].data());
		}
	}
}

void DrawPlayerBlob(int xpos, int ypos, int col)
{
	DrawCross(xpos, ypos, col, 0);
	POINT Player = { Me->pPath->xPos,Me->pPath->yPos };
	MapToScreenCoords(Player);
	ScreenToAutomapRelative(&Player, Player.x, Player.y);
	D2GFX_DrawLine(Player.x, Player.y, xpos, ypos, col, 0xff);
}

VOID MapToScreenCoords(POINT& rMapPosition)
{
	D2COMMON_MapToAbsScreen(&rMapPosition.x, &rMapPosition.y);
	rMapPosition.x -= D2CLIENT_GetMouseXOffset();
	rMapPosition.y -= D2CLIENT_GetMouseYOffset();
}

VOID ScreenToAutomapRelative(POINT* pos, INT x, INT y)
{
	x += D2CLIENT_GetMouseXOffset();
	y += D2CLIENT_GetMouseYOffset();
	Funcs->ScreenToAutomap(pos, (y * 2 + x) / 32, (y * 2 - x) / 32);
}