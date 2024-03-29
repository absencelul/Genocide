#include "Hack.h"

Unit::Unit()
{
}


Unit::~Unit()
{
}

//Basic checks for functions.
bool Unit::CheckReady(bool TownCheck, bool CheckCasting)
{
	if (TownCheck)
		if (TP->IsTown(Me))
			return false;

	if (!ClientReady)
		return false;

	if (CheckCasting)
		for (int i = 7; i < 16; i++)
			if (Me->dwMode == i)
				return false;

	return true;
}

//Finds if player has merc
LPUNITANY Unit::FindMercUnit(LPUNITANY pOwner)
{
	const MercID _MercIds[4] = { MERC_A1, MERC_A2, MERC_A3, MERC_A5 };

	for (LPROOM1 pRoom1 = Me->pAct->pRoom1; pRoom1; pRoom1 = pRoom1->pRoomNext)
		for (LPUNITANY pUnit = pRoom1->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
			for (const auto id : _MercIds)
				if (pUnit->dwTxtFileNo == id && D2CLIENT_GetMonsterOwner(pUnit->dwUnitId) == pOwner->dwUnitId && pUnit->dwMode != NPC_MODE_DEAD || NPC_MODE_DEATH)
					return pUnit;

	return NULL;
}

//Finds nearest unit
UnitAny* Unit::FindUnit()
{
	UnitAny* pMe = D2CLIENT_GetPlayerUnit();
	if (!pMe)
		return false;

	int nDist = 50;
	UnitAny* pRet = 0;
	for (RosterUnit* pRoster = *p_D2CLIENT_PlayerUnitList; pRoster; pRoster = pRoster->pNext)
	{
		if (pRoster->dwUnitId == pMe->dwUnitId)
			continue;
		UnitAny* pTarget = D2CLIENT_FindServerSideUnit(pRoster->dwUnitId, UNIT_TYPE_PLAYER);
		if (!pTarget)
			continue;

		if (GetRelation(pTarget) != 4)
			continue;

		if (pTarget->dwMode == PLAYER_MODE_DEAD || pTarget->dwMode == PLAYER_MODE_DEATH)
			continue;

		if (TP->IsTown(pTarget))
			continue;

		int tDist = D2MATH_GetDistance(pMe, pTarget->pPath->xPos, pTarget->pPath->yPos);

		if (tDist < nDist)
		{
			nDist = tDist;
			pRet = pTarget;
		}
	}

	return pRet;
}

//Finds nearest hostile unit
POINT Unit::FindUnitHammer()
{
	POINT Target = { NULL };
	int Dist = 70;
	for (int i = 0; i < Players.GetSize(); i++)
	{
		//LPUNITANY Unit = (LPUNITANY)GetUnit(Players[i]->UnitId, UNIT_TYPE_PLAYER);
		UnitAny* Unit = D2CLIENT_FindServerSideUnit(Players[i]->UnitId, UNIT_TYPE_PLAYER);

		if (!Unit)
			continue;

		if (TP->IsTown(Unit))
			continue;

		if (Unit->dwMode == PLAYER_MODE_WALK_OUTTOWN || Unit->dwMode == PLAYER_MODE_RUN)
			AA->SetBlind(Unit->pPath->xTarget, Unit->pPath->yTarget, Unit->pPath->xTarget, Unit->pPath->yTarget, Unit->dwUnitId);

		if (Unit->dwMode == PLAYER_MODE_DEAD || Unit->dwMode == PLAYER_MODE_DEATH)
			continue;

		if (Math::CalculateDistance(Me->pPath->xPos, Me->pPath->yPos, Unit->pPath->xPos, Unit->pPath->yPos) >= Dist)
			continue;

		if (GetRelation(Unit) & PVP_HOSTILED_YOU || GetRelation(Unit) & PVP_HOSTILED_BY_YOU)
		{
			Target.x = Players[i]->HammerX;
			Target.y = Players[i]->HammerY;

			//Dist = Math::CalcDist(Me, Unit, false);
			Dist = Math::CalculateDistance(Me->pPath->xPos, Me->pPath->yPos, Unit->pPath->xPos, Unit->pPath->yPos);
		}
	}

	return Target;
}

POINT Unit::FindUnitBS()
{
	POINT ptTarget = { NULL };
	int Dist = 50;
	for (int i = 0; i < Players.GetSize(); i++)
	{
		UnitAny* pUnit = (UnitAny*)GetUnit(Players[i]->UnitId, UNIT_TYPE_PLAYER);

		if (!pUnit)
			continue;

		if (TP->IsTown(pUnit))
			continue;

		if (pUnit->dwMode == PLAYER_MODE_DEAD || pUnit->dwMode == PLAYER_MODE_DEATH)
			continue;

		if (D2MATH_GetDistance(Me, pUnit->pPath->xPos, pUnit->pPath->yPos) >= Dist)
			continue;

		if (GetRelation(pUnit) & PVP_HOSTILED_YOU || GetRelation(pUnit) & PVP_HOSTILED_BY_YOU)
		{
			ptTarget.x = Players[i]->X;
			ptTarget.y = Players[i]->Y;

			//Dist = Math::CalcDist(Me, Unit, false);
			Dist = D2MATH_GetDistance(Me, pUnit->pPath->xPos, pUnit->pPath->yPos);
		}
	}

	return ptTarget;
}

//Finds Unit by id.
LPROSTERUNIT Unit::FindPartyById(DWORD dwUnitId)
{
	for (RosterUnit* pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
		if (pUnit->dwUnitId == dwUnitId)
			return pUnit;

	return NULL;
}

//Finds Unit by name.
LPROSTERUNIT Unit::FindPartyByName(LPSTR szName)
{

	for (LPROSTERUNIT pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
		if (!_stricmp(szName, pUnit->szName))
			return pUnit;

	return NULL;
}

int Unit::GetRelation(UnitAny* unit)
{
	UnitAny* player = D2CLIENT_GetPlayerUnit();
	RosterUnit* roster;

	//Neutral = 2, Partied = 3, You = 1, Hostile = 4
	if (!unit || !player)
		return 2;

	RosterUnit* playerRoster = FindPartyById(player->dwUnitId);

	switch (unit->dwType)
	{
	case 0://Player

		// Check if we are the unit.
		if (unit->dwUnitId == player->dwUnitId)
			return 1;//You

		// Check if we are in a party with unit.
		roster = FindPartyById(unit->dwUnitId);
		if (playerRoster && roster && playerRoster->wPartyId == roster->wPartyId && roster->wPartyId != 0xFFFF)
			return 3;//Partied

		// Check if unit is hostile towards us
		if (D2CLIENT_TestPvpFlag_I(unit->dwUnitId, player->dwUnitId, 8) || D2CLIENT_TestPvpFlag_I(player->dwUnitId, unit->dwUnitId, 8))
			return 4;

		return 2;
		break;
	case 1://Monster or NPC or Summon
	case 3://Missile

		if (unit->dwOwnerId < 0 || unit->dwOwnerType < 0 || unit->dwOwnerType > 5)
			return 4;

		// Find the owner of the unit.
		UnitAny* owner = D2CLIENT_FindServerSideUnit(unit->dwOwnerId, unit->dwOwnerType);
		if (!owner || owner->dwType != unit->dwOwnerType || owner->dwUnitId != unit->dwOwnerId)
			return 4;

		return GetRelation(owner);
		break;
	}

	return 2;//Neutral
}

//Gets Player class based upon id short class or long class
char * Unit::PlayerClass(DWORD ClassId, bool Short)
{
	switch (ClassId)
	{
	case 0:

		if (Short)
			return "Ama";
		else
			return "Amazon";

		break;

	case 1:

		if (Short)
			return "Sorc";
		else
			return "Sorceress";

		break;

	case 2:

		if (Short)
			return "Nec";
		else
			return "Necromancer";

		break;

	case 3:

		if (Short)
			return "Pal";
		else
			return "Paladin";

		break;

	case 4:

		if (Short)
			return "Barb";
		else
			return "Barbarian";

		break;

	case 5:

		if (Short)
			return "Dru";
		else
			return "Druid";

		break;

	case 6:

		if (Short)
			return "Asn";
		else
			return "Assassin";

		break;
	}

	return "null";
}

LPLEVEL Unit::GetUnitLevel(LPUNITANY pUnit)
{
	if (pUnit)
		if (pUnit->pPath)
			if (pUnit->pPath->pRoom1)
				if (pUnit->pPath->pRoom1->pRoom2)
					return pUnit->pPath->pRoom1->pRoom2->pLevel;

	return FALSE;
}

/*LPPRESETUNIT Unit::FindPresetUnit(DWORD dwClassId, DWORD dwType, LPROOM2 *pRoom)
{
	for (LPROOM2 pRoom2 = GetUnitLevel(Me)->pRoom2First; pRoom2; pRoom2 = pRoom2->pRoom2Next)
	{
		for (LPPRESETUNIT pUnit = pRoom2->pPreset; pUnit; pUnit = pUnit->pPresetNext)
		{
			if (pUnit->dwType == dwType && pUnit->dwTxtFileNo == dwClassId)
			{
				*pRoom = pRoom2;
				return pUnit;
			}
		}
	}

	return NULL;
}*/