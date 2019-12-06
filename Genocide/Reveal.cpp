#include "Hack.h"

void ResetRevealed()
{
	revealedGame = false;
	for (int act = 0; act < 6; act++)
		revealedAct[act] = false;
	for (int level = 0; level < 255; level++)
		revealedLevel[level] = false;
}

void RevealAutomap()
{
	// Check if we have already revealed the game.
	if (revealedGame)
		return;

	// Iterate every act and reveal it.
	for (auto act = 1; act <= ((*p_D2CLIENT_ExpCharFlag) ? 5 : 4); act++)
		RevealAct(act);
	revealedGame = true;
}

void RevealAct(int act)
{
	// Make sure we are given a valid act
	if (act < 1 || act > 5)
		return;

	// Check if the act is already revealed
	if (revealedAct[act])
		return;

	UnitAny* player = D2CLIENT_GetPlayerUnit();
	if (!player || !player->pAct)
		return;

	// Initalize the act incase it is isn't the act we are in.
	int actIds[6] = { 1, 40, 75, 103, 109, 137 };
	Act* pAct = D2COMMON_LoadAct(act - 1, player->pAct->dwMapSeed, *p_D2CLIENT_ExpCharFlag, 0, D2CLIENT_GetDifficulty(), NULL, actIds[act - 1], D2CLIENT_LoadAct_1, D2CLIENT_LoadAct_3);
	if (!pAct || !pAct->pMisc)
		return;

	// Iterate every level for the given act.
	for (int level = actIds[act - 1]; level < actIds[act]; level++) {
		Level* pLevel = GetLevels(pAct, level);
		if (!pLevel)
			continue;
		if (!pLevel->pRoom2First)
			D2COMMON_InitLevel(pLevel);
		RevealLevel(pLevel);
	}
	InitAutomapLayer(player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo);
	D2COMMON_UnloadAct(pAct);
	revealedAct[act] = true;
}

void RevealLevel(Level* pLevel)
{
	// Basic sanity checks to ensure valid level
	if (!pLevel || pLevel->dwLevelNo < 0 || pLevel->dwLevelNo > 255)
		return;

	// Check if the level has been previous revealed.
	if (revealedLevel[pLevel->dwLevelNo])
		return;

	InitAutomapLayer(pLevel->dwLevelNo);

	// Iterate every room in the level.
	for (Room2* room = pLevel->pRoom2First; room; room = room->pRoom2Next) {
		bool roomData = false;

		//Add Room1 Data if it is not already there.
		if (!room->pRoom1) {
			D2COMMON_AddRoomData(pLevel->pMisc->pAct, pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
			roomData = true;
		}

		//Make sure we have Room1
		if (!room->pRoom1)
			continue;

		//Reveal the room
		D2CLIENT_RevealAutomapRoom(room->pRoom1, TRUE, *p_D2CLIENT_AutomapLayer);

		//Reveal the presets
		RevealRoom(room);

		//Remove Data if Added
		if (roomData)
			D2COMMON_RemoveRoomData(pLevel->pMisc->pAct, pLevel->dwLevelNo, room->dwPosX, room->dwPosY, room->pRoom1);
	}

	revealedLevel[pLevel->dwLevelNo] = true;
}

void RevealRoom(Room2* pRoom)
{
	for (PresetUnit* pUnit = pRoom->pPreset; pUnit; pUnit = pUnit->pPresetNext)
	{
		int nCell = -1;

		if (pUnit->dwType == UNIT_TYPE_NPC)
		{
			// Izual Check
			if (pUnit->dwTxtFileNo == 256) nCell = 300;
			// Hephasto Check
			if (pUnit->dwTxtFileNo == 745) nCell = 745;
		}
		else if (pUnit->dwType == UNIT_TYPE_OBJECT)
		{
			// Uber Chest in Lower Kurast Check
			if (pUnit->dwTxtFileNo == 580 && pRoom->pLevel->dwLevelNo == 79)
				nCell = 9;
			// Countess Chest Check
			if (pUnit->dwTxtFileNo == 371)
				nCell = 301;
			// Act 2 Orifice Check
			else if (pUnit->dwTxtFileNo == 152)
				nCell = 300;
			// Frozen Anya Check
			else if (pUnit->dwTxtFileNo == 460)
				nCell = 1468;
			// Canyon / Arcane Waypoint Check
			if ((pUnit->dwTxtFileNo == 402) && (pRoom->pLevel->dwLevelNo == 46))
				nCell = 0;
			// Hell Forge Check
			if (pUnit->dwTxtFileNo == 376)
				nCell = 376;
			// If it isn't special, check for a preset.
			if (nCell == -1 && pUnit->dwTxtFileNo <= 572) {
				ObjectTxt* obj = D2COMMON_GetObjectTxt(pUnit->dwTxtFileNo);
				if (obj)
					nCell = obj->nAutoMap;//Set the cell number then.
			}
		}
		else if (pUnit->dwType == UNIT_TYPE_TILE)
		{
			LevelList* level = new LevelList;
			for (RoomTile* tile = pRoom->pRoomTiles; tile; tile = tile->pNext)
			{
				if (*(tile->nNum) == pUnit->dwTxtFileNo)
				{
					level->levelId = tile->pRoom2->pLevel->dwLevelNo;
					break;
				}
			}
			level->x = (pUnit->dwPosX + (pRoom->dwPosX * 5));
			level->y = (pUnit->dwPosY + (pRoom->dwPosY * 5));
			level->act = pRoom->pLevel->pMisc->pAct->dwAct;
			automapLevels.push_back(level);
		}
		//Draw the cell if wanted.
		if ((nCell > 0) && (nCell < 1258))
		{
			AutomapCell* cell = D2CLIENT_NewAutomapCell();
			cell->nCellNo = nCell;
			int x = (pUnit->dwPosX + (pRoom->dwPosX * 5));
			int y = (pUnit->dwPosY + (pRoom->dwPosY * 5));
			cell->xPixel = (((x - y) * 16) / 10) + 1;
			cell->yPixel = (((y + x) * 8) / 10) - 3;
			D2CLIENT_AddAutomapCell(cell, &((*p_D2CLIENT_AutomapLayer)->pObjects));
		}
	}
	return;
}

VOID DeleteShrines()
{
	for (INT i = 0; i < 2; i++)
	{
		for (INT j = 0; j < TOTALSHRINES; j++)
		{
			if (MyShrineCells[i][j])
				DeleteCellFile(MyShrineCells[i][j]);
		}
	}

	if (WaypointCell)
		DeleteCellFile(WaypointCell);

	if (MinimapObjectCell)
		DeleteCellFile(MinimapObjectCell);
}

VOID InitShrines()
{
	CHAR FileName[MAX_PATH];

	for (INT i = 0; i < 2; i++)
	{
		for (INT j = 0; j < TOTALSHRINES; j++)
		{
			wsprintfA(GetShrinesFolder(FileName, '\\'), "MINISHRINE%.2d.BMP" + (i ? 0 : 4), j);
			MyShrineCells[i][j] = InitCellFile(LoadBMPCellFile(FileName));
		}
	}

	wsprintfA(GetShrinesFolder(FileName, '\\'), "WAYPOINT.BMP");
	WaypointCell = InitCellFile(LoadBMPCellFile(FileName));

	wsprintfA(GetShrinesFolder(FileName, '\\'), "BLOBCELL.BMP");
	MinimapObjectCell = InitCellFile(LoadBMPCellFile(FileName));

	if (!MinimapObjectCell)
	{
		wsprintfA(GetShrinesFolder(FileName, '\\'), "BLOBDOT.BMP");
		MinimapObjectCell = InitCellFile(LoadBMPCellFile(FileName));
	}
}

/*VOID Reveal::InitTCList()
{
	LPMONSTERTXT mon;
	LPSUPERUNIQUETXT sup;

	for (INT i = 0; mon = D2CLIENT_GetMonsterTxt(i); i++)
	{
		TCTALLYS tallys;
		memset(&tallys, 0, sizeof(tallys));

		for (INT j = 0; j < 4; j++)
		{
			INT tcno = mon->tcs[D2CLIENT_GetDifficulty()][j];
			GetHighestTC(tcno, &tallys);
		}

		MonsterTCs[i] = tallys.groupnos[1];
	}

	for (INT j = 0; sup = D2COMMON_GetSuperUniqueTxt(j); j++)
	{
		TCTallys tallys;
		memset(&tallys, 0, sizeof(tallys));

		INT tcno = sup->tcs[D2CLIENT_GetDifficulty()];
		GetHighestTC(tcno, &tallys);
		SuperUniqueTCs[j] = tallys.groupnos[1];
	}
}*/

Level* GetLevels(Act* pAct, DWORD dwLevelNo)
{
	//Insure that the shit we are getting is good.
	if (dwLevelNo < 0 || !pAct)
		return NULL;

	//Loop all the levels in this act
	for (Level* pLevel = pAct->pMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel)
	{
		//Check if we have reached a bad level.
		if (!pLevel)
			break;

		//If we have found the level, return it!
		if (pLevel->dwLevelNo == dwLevelNo && pLevel->dwPosX > 0)
			return pLevel;
	}

	return D2COMMON_GetLevel(pAct->pMisc, dwLevelNo);
}

AutomapLayer* InitAutomapLayer(DWORD levelno)
{
	//Get the layer for the level.
	AutomapLayer2* pLayer = D2COMMON_GetLayer(levelno);

	//Ensure we have found the Layer.
	if (!pLayer)
		return false;

	//Initalize the layer!
	return (AutomapLayer*)Stubs::D2CLIENT_InitAutomapLayer_STUB(pLayer->nLayerNo);
}

VOID FASTCALL OverrideShrine(LPAUTOMAPCELL New, LPAUTOMAPCELL Current)
{
	if ((New->nCellNo < CELLNO_MYSHRINES) || (New->nCellNo >= (CELLNO_MYSHRINES + TOTALSHRINES)))
		return;

	if (Current->nCellNo >= New->nCellNo)
		return;

	Current->fSaved = New->fSaved;
	Current->nCellNo = New->nCellNo;
}