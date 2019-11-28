#include "Hack.h"

void RevealMonsters()
{
	if (!show_monsters) return;

	UnitAny* player = D2CLIENT_GetPlayerUnit();

	if (!player || !player->pAct || player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo == 0)
		return;

	for (Room1* room1 = player->pAct->pRoom1; room1; room1 = room1->pRoomNext)
	{
		for (UnitAny* pUnit = room1->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
		{
			POINT automapLoc;
			ScreenToAutomap(&automapLoc, pUnit->pPath->xPos, pUnit->pPath->yPos);

			// Draw monster on automap
			if (pUnit->dwType == 1 && IsValidMonster(pUnit)) {
				auto color = normal_monster;
				if (pUnit->pMonsterData->fBoss)
					color = boss_monster;
				if (pUnit->pMonsterData->fChamp)
					color = champion_monster;
				if (pUnit->pMonsterData->fMinion)
					color = minion_monster;
				//Determine immunities
				std::string szImmunities[] = { "˙c7i", "˙c8i", "˙c1i", "˙c9i", "˙c3i", "˙c2i" };
				DWORD dwImmunities[] = { 36,37,39,41,43,45 };
				std::string immunityText;
				for (int n = 0; n < 6; n++) {
					auto nImm = D2COMMON_GetUnitStat(pUnit, dwImmunities[n], 0);
					if (nImm >= 100)
						immunityText += szImmunities[n];
				}
				if (immunityText.length() > 0)
					TextHook(automapLoc.x, automapLoc.y - 8, White, Center, 6, immunityText);
				DrawPlayerBlob(automapLoc.x, automapLoc.y, color);
			}
		}
	}
}