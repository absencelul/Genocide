#include "Hack.h"

void DrawAutomap()
{
	GameInfo();
	xVector();
	PlayerInfo();
	FCPointer();
	RevealMissiles();
	RevealMonsters();
}

void GameInfo()
{
	int y = 0;
	std::list<std::string> GameInfoList;

	//AddStringToList(GameInfoList, "FPS: %d, Skip: %d, Ping: %d", FPS, Skip, Ping);
	if (strlen(D2CLIENT_GetGameInfo()->szGameName) > 0)
		AddStringToList(GameInfoList, "Game: %s", D2CLIENT_GetGameInfo()->szGameName);
	if (strlen(D2CLIENT_GetGameInfo()->szGamePassword) > 0)
		AddStringToList(GameInfoList, "Password: %s", D2CLIENT_GetGameInfo()->szGamePassword);
	AddStringToList(GameInfoList, "%s", UnicodeToAnsi(D2CLIENT_GetLevelName_I(D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo)));
	AddStringToList(GameInfoList, "v 1.14d", Difficulty().c_str());
	if (strcmp(Difficulty().c_str(), "Normal") != 0)
		AddStringToList(GameInfoList, "Difficulty: %s", Difficulty().c_str());
	AddStringToList(GameInfoList, "EXPANSION", Difficulty().c_str());
	/*if (strlen(D2CLIENT_GetGameInfo()->szGameServerIp) > 0)
		AddStringToList(GameInfoList, "%s", D2CLIENT_GetGameInfo()->szGameServerIp);*/

	for (auto& it : GameInfoList)
		TextHook(*p_D2CLIENT_ScreenSizeX - 18, y += 16, Gold, Right, 1, false, "%s", it.c_str());
}

void xVector()
{
	if (!ClientReady)
		return;

	POINT Player, Position, Last = { 0, 0 };

	ScreenToAutomap(&Player, Me->pPath->xPos, Me->pPath->yPos);

	if (!Players.IsEmpty())
	{
		LPUNITANY Unit = (LPUNITANY)GetUnit(Players[CurrentTarget]->UnitId, UNIT_TYPE_PLAYER);

		if (Unit)
		{
			ScreenToAutomap(&Position, Unit->pPath->xPos, Unit->pPath->yPos);
			D2GFX_DrawLine(Player.x, Player.y, Position.x, Position.y, (DWORD)DarkGreen, -1);
			DrawPlayerBlob(Position.x, Position.y, (DWORD)DarkGreen);
		}
	}
}

void PlayerInfo()
{
	for (int i = 0; i < Players.GetSize(); i++) {
		LPUNITANY Unit = (LPUNITANY)GetUnit(Players[i]->UnitId, UNIT_TYPE_PLAYER);

		if (!Unit)
			continue;

		LPUNITANY Merc = FindMercUnit(Unit);

		if (!Merc)
			continue;

		POINT automapLoc;
		ScreenToAutomap(&automapLoc, Merc->pPath->xPos, Merc->pPath->yPos);

		if (Merc)
		{
			TextHook(automapLoc.x, automapLoc.y - 8, Gold, Center, 6, true, "Merc");
			DrawCross(Merc->pPath->xPos, Merc->pPath->yPos, Yellow, 1);
		}
	}

	for (int i = 0; i < Players.GetSize(); i++) {
		LPUNITANY Unit = (LPUNITANY)GetUnit(Players[i]->UnitId, UNIT_TYPE_PLAYER);

		if (!Unit)
			continue;

		POINT Position;
		ScreenToAutomap(&Position, Unit->pPath->xPos, Unit->pPath->yPos);
		TextColor Color = Green;

		if (Players[i]->Life < 40)
			Color = Red;

		if (Players[i]->Life < 80 && Players[i]->Life >= 40)
			Color = Yellow;

		if (Unit && Players[i]->Life >= 1)
			TextHook(Position.x, Position.y - 10, Color, Center, 6, true, "[%d%%]", Players[i]->Life);
		if (Unit && !GetUnitState(Unit, AFFECT_BATTLEORDERS))
			TextHook(Position.x, Position.y + 9, Green, Center, 6, true, "[OUT]");
		else if (Unit && GetUnitState(Unit, AFFECT_BATTLEORDERS))
			TextHook(Position.x, Position.y + 9, GetUnitState(Unit, AFFECT_SHOUT) ? Red : Yellow, Center, 6, true, GetUnitState(Unit, AFFECT_SHOUT) ? "[BARB]" : "[BO]");

		if (Unit && Unit->dwMode == PLAYER_MODE_STAND_OUTTOWN)
			TextHook(Position.x, Position.y + 17, Green, Center, 6, true, "[STAND]");
		else if (Unit && Unit->dwMode == PLAYER_MODE_RUN)
			TextHook(Position.x, Position.y + 17, Yellow, Center, 6, true, "[RUN]");
		else if (Unit && Unit->dwMode == PLAYER_MODE_WALK_OUTTOWN)
			TextHook(Position.x, Position.y + 17, Red, Center, 6, true, "[WALK]");
		else if (Unit && Unit->dwMode == PLAYER_MODE_CAST)
			TextHook(Position.x, Position.y + 17, Yellow, Center, 6, true, "[CAST]");
	}

	for (LPROOM1 pRoom1 = Me->pAct->pRoom1; pRoom1; pRoom1 = pRoom1->pRoomNext)
	{
		for (LPUNITANY pUnit = pRoom1->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
		{
			POINT Position;
			ScreenToAutomap(&Position, pUnit->pObjectPath->dwPosX, pUnit->pObjectPath->dwPosY);
			if (pUnit->dwType == UNIT_TYPE_OBJECT && pUnit->dwTxtFileNo == 59)
			{
				TextHook(pUnit->pObjectPath->dwPosX - 4, pUnit->pObjectPath->dwPosY - 2, Blue, Center, 6, "%s", pUnit->pObjectData->szOwner);
				for (int i = 0; i < Players.GetSize(); i++) {
					if (!_stricmp(Players[i]->PlayerName, pUnit->pObjectData->szOwner)) {
						TextHook(Position.x + 1, Position.y + 5, Green, Center, 6, true, "%d%%", Players[i]->Life);
					}
				}
			}
		}
	}
}

void DrawCross(INT X, INT Y, DWORD Color, BOOL Automap)
{
	POINT Position = { X, Y };

	if (Automap)
	{
		if (!p_D2CLIENT_AutomapOn)
			return;

		ScreenToAutomap(&Position, X, Y);
	}

	CHAR Lines[][2] = { 0, -2, 4, -4, 8, -2, 4, 0, 8, 2, 4, 4, 0, 2, -4, 4, -8, 2, -4, 0, -8, -2, -4, -4, 0, -2 };
	//for (auto& i : Lines)
		//D2GFX_DrawLine(Position.x + i[0], Position.y + i[1], Position.x + (i+1)[0], Position.y + (i+1)[1], Color, -1);
	for (INT i = 0; i < ArraySize(Lines) - 1; i++)
		D2GFX_DrawLine(Position.x + Lines[i][0], Position.y + Lines[i][1], Position.x + Lines[i + 1][0], Position.y + Lines[i + 1][1], Color, -1);
}

void FCPointer()
{
	if (IsTown(Me))
		return;

	POINT Mouse = { *(long*)& p_D2CLIENT_MouseX, *(long*)& p_D2CLIENT_MouseY };
	ScreenToAutoMap(&Mouse);

	POINT Player = { Me->pPath->xPos, Me->pPath->yPos };

	INT Angle = CalculateAngle(Player, Mouse);
	INT Radius = FcDistance;
	POINT Cast = { NULL };

	while (TRUE) {
		Cast = CalculatePointOnTrack(Player, Radius--, Angle);

		if (!D2COMMON_CheckCollision(Me->pPath->pRoom1, Cast.x, Cast.y, 2))
			if (::abs(Me->pPath->xPos - Cast.x) <= FcDistance && ::abs(Me->pPath->yPos - Cast.y) <= FcDistance)
				break;
	}

	if (GetCurrentSkill(false) == D2S_TELEPORT) {
		POINT Start = { 0, 0 };
		POINT End = { 0, 0 };

		ScreenToAutomap(&Start, Player.x, Player.y);
		ScreenToAutomap(&End, Cast.x, Cast.y);
		D2GFX_DrawLine(Start.x, Start.y, End.x, End.y, 168, 0);
		TextHook(End.x + 1, End.y + 3, Yellow, None, 4, true, "x");
	}
}

void AddStringToList(std::list<std::string>& StringList, std::string lpFormat, ...)
{
	char szBuffer[4096];
	va_list arg;
	va_start(arg, lpFormat);
	vsprintf(szBuffer, lpFormat.c_str(), arg);
	va_end(arg);

	StringList.push_back(szBuffer);
}