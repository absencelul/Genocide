#include "Hack.h"

Missiles::Missiles()
{
}


Missiles::~Missiles()
{
}

void Missiles::RevealMissiles()
{
	if (!ShowMissiles)
		return;

	UnitAny* player = D2CLIENT_GetPlayerUnit();

	if (!player || !player->pAct || player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo == 0)
		return;

	for (Room1* room1 = player->pAct->pRoom1; room1; room1 = room1->pRoomNext)
	{
		for (UnitAny* pUnit = room1->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
		{
			POINT automapLoc;
			ScreenToAutomap(&automapLoc, pUnit->pPath->xPos, pUnit->pPath->yPos);

			if (pUnit->dwType == UNIT_TYPE_MISSILE)
			{
				auto color = 255;

				switch (GetRelation(pUnit)) {
				case 0:
					continue;
					break;
				case 1://Me
					color = MyMissile;
					break;
				case 2://Neutral
					color = NeutralMissile;
					break;
				case 3://Partied
					color = FriendMissile;
					break;
				case 4://Hostile
					color = EnemyMissile;
					break;
				}

				BoxHook(automapLoc.x - 1, automapLoc.y - 1, 4, 4, color, BTHighlight);
			}
		}
	}
}