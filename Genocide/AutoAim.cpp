#include "DllMain.h"

void EnumeratePlayers()
{
	CArrayEx<PTARGETINFO, PTARGETINFO> TempInfo;
	INT nNewTarget = 0;

	if (Players.IsLocked)
		return;

	Players.Lock();

	for (INT i = 0; i < Players.GetSize(); i++)
		TempInfo.Add(Players[i]);

	Players.RemoveAll();
	
	for (LPROSTERUNIT pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
	{
		BOOL Found = FALSE;

		if (pUnit->dwUnitId == Me->dwUnitId)
			continue;

		if (pUnit->wLevel <= 8)
			continue;

		PTARGETINFO pInfo = new TARGETINFO;
		LPUNITANY pPlayer = NULL;

		strcpy(pInfo->PlayerName, pUnit->szName);
		pInfo->UnitId = pUnit->dwUnitId;
		pInfo->Level = (BYTE)pUnit->wLevel;
		pInfo->ClassId = pUnit->dwClassId;
		pInfo->Life = (BYTE)pUnit->dwPartyLife;

		for (INT i = 0; i < 6; i++)
			pInfo->Resists[i] = 0;

		for (INT i = 0; i < TempInfo.GetSize(); i++)
		{
			if (TempInfo[i]->UnitId == pUnit->dwUnitId)
			{
				pInfo->Life = TempInfo[i]->Life;
				pInfo->X = TempInfo[i]->X;
				pInfo->Y = TempInfo[i]->Y;
				pInfo->HammerX = TempInfo[i]->HammerX;
				pInfo->HammerY = TempInfo[i]->HammerY;

				for (INT j = 0; j < 6; j++)
					pInfo->Resists[j] = TempInfo[i]->Resists[j];
			}
		}

		pPlayer = (LPUNITANY)GetUnit(pUnit->dwUnitId, UNIT_TYPE_PLAYER);
		if (pPlayer)
		{
			pInfo->Resists[0] = GetUnitStat(pPlayer, STAT_FIREABSORB);
			pInfo->Resists[1] = GetUnitStat(pPlayer, STAT_FIREABSORBPERCENT);
			pInfo->Resists[2] = GetUnitStat(pPlayer, STAT_COLDABSORB);
			pInfo->Resists[3] = GetUnitStat(pPlayer, STAT_COLDABSORBPERCENT);
			pInfo->Resists[4] = GetUnitStat(pPlayer, STAT_LIGHTNINGABSORB);
			pInfo->Resists[5] = GetUnitStat(pPlayer, STAT_LIGHTNINGABSORBPERCENT);
		}

		Players.Add(pInfo);
		if (!TempInfo.IsEmpty() && pInfo->UnitId == TempInfo[CurrentTarget]->UnitId)
			nNewTarget = Players.GetSize() - 1;
	}

	for (INT i = 0; i < TempInfo.GetSize(); i++)
		delete TempInfo[i];

	CurrentTarget = nNewTarget;
	Players.Unlock();

}

void Attack(bool Left)
{
	if (!CheckReady(true, true))
		return;

	LPUNITANY Unit = (LPUNITANY)GetUnit(Players[CurrentTarget]->UnitId, UNIT_TYPE_PLAYER);

	if (!Unit)
		return;

	if (Unit->dwMode == PLAYER_MODE_RUN || Unit->dwMode == PLAYER_MODE_WALK_OUTTOWN)
		SetBlind(Unit->pPath->xTarget, Unit->pPath->yTarget, NULL, NULL, Unit->dwUnitId);

	if (Unit->dwMode == PLAYER_MODE_DEAD || Unit->dwMode == PLAYER_MODE_DEATH)
		return;

	DWORD Dist = CalculateDistance(Me->pPath->xPos, Me->pPath->yPos, Players[CurrentTarget]->X, Players[CurrentTarget]->Y);

	if (Dist == 0)
		return;

	AttackStruct Attack;

	Attack.dwAttackType = Left ? 0xE5 : 0x66;

	Attack.dwTargetX = Players[CurrentTarget]->X;
	Attack.dwTargetY = Players[CurrentTarget]->Y;

	Attack.lpPlayerUnit = Me;

	Attack.lpTargetUnit = NULL;

	D2CLIENT_Attack(&Attack, false);

}

void AttackNearest(bool Left)
{
	if (!CheckReady(true, true))
		return;

	POINT Nearest = FindUnit();

	if (Nearest.x == 0 || Nearest.x == NULL)
		return;

	DWORD Dist = CalculateDistance(Me->pPath->xPos, Me->pPath->yPos, Nearest.x, Nearest.y);

	if (Dist == 0)
		return;

	AttackStruct Attack;

	Attack.dwAttackType = Left ? 0xE5 : 0x66;

	Attack.dwTargetX = Nearest.x;
	Attack.dwTargetY = Nearest.y;

	Attack.lpPlayerUnit = Me;

	Attack.lpTargetUnit = NULL;

	D2CLIENT_Attack(&Attack, false);

}

void SetBlind(WORD x, WORD y, WORD xHammer, WORD yHammer, DWORD UnitId)
{
	for (int i = 0; i < Players.GetSize(); i++)
	{
		if (Players[i]->UnitId != UnitId)
			continue;

		Players[i]->X = x + Blinds[0].at(Blindz[0]);
		Players[i]->Y = y + Blinds[1].at(Blindz[0]);
		Players[i]->HammerX = x;
		Players[i]->HammerY = y;
	}
}

void PrevTar()
{
	if (Players.IsEmpty())
		return;

	if (CurrentTarget == 0)
		CurrentTarget = Players.GetSize() - 1;
	else
		CurrentTarget--;
}

void NextTar()
{
	if (Players.IsEmpty())
		return;

	if (CurrentTarget == Players.GetSize() - 1)
		CurrentTarget = 0;
	else
		CurrentTarget++;
}

/*DWORD __fastcall GameAttack(AttackStruct* pAttack)
{
	WORD SelectedSkill = GetCurrentSkill(false);
	POINT Target = { (pAttack->dwTargetX), (pAttack->dwTargetY) };

	if (!pAttack || !pAttack->lpTargetUnit || pAttack->lpTargetUnit->dwType != UNIT_TYPE_NPC) return (DWORD)-1;
	if (pAttack->dwAttackType == ATTACKTYPE_LEFT)	 pAttack->dwAttackType = ATTACKTYPE_SHIFTLEFT;
	if (pAttack->dwAttackType == ATTACKTYPE_RIGHT)	 pAttack->dwAttackType = ATTACKTYPE_SHIFTRIGHT;


	return NULL;
}*/