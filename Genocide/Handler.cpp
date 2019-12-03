#include "Hack.h"

void OnGameLoop()
{
	if (!ClientReady)
		GameStart();

	EnumeratePlayers();

	if (ClientReady && AutoRevealAutomap) RevealAutomap();

	if (!p_D2CLIENT_UIMode[UI_INVENTORY] || p_D2CLIENT_UIMode[UI_TRADE])
	{
		//RemoveInventoryPatch();

		ViewingUnit = NULL;
	}

	if (ViewingUnit && ViewingUnit->dwUnitId)
		if (!GetUnit(ViewingUnit->dwUnitId, UNIT_TYPE_PLAYER))
		{
			ViewingUnit = NULL;
			D2CLIENT_SetUIVar(UI_INVENTORY, 1, 0);
		}
}

void GameStart()
{
	ClientReady = true;
	ResetRevealed();
	automapLevels.clear();
}

void OnGameEnd()
{
	ClientReady = false;
	BoTimer = 0;
	killCountGame = 0;
}