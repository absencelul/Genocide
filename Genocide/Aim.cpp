#include "Hack.h"

PACKETATTACK Attacks[] =
{
	{ D2S_ATTACK, true},
	{ D2S_AMPLIFYDAMAGE, false },
	{ D2S_BERSERK, false },
	{ D2S_BLIZZARD,false },
	{ D2S_BONESPEAR,false },
	{ D2S_BONESPIRIT,false },
	{ D2S_CHAINLIGHTNING,false },
	{ D2S_CHARGE,false },
	{ D2S_CHARGEDSTRIKE,false },
	{ D2S_CONCENTRATE, true },
	{ D2S_DOUBLETHROW,true },
	{ D2S_FIREBALL, false },
	{ D2S_FIREBOLT, false },
	{ D2S_WAKEOFFIRE,false },
	{ D2S_FISTOFTHEHEAVENS, false },
	{ D2S_GLACIALSPIKE,false },
	{ D2S_GUIDEDARROW,false },
	{ D2S_ICEBLAST,false },
	{ D2S_LIGHTNING,false },
	{ D2S_LIGHTNINGFURY,false },
	{ D2S_LIGHTNINGSENTRY,false },
	{ D2S_METEOR,false },
	{ D2S_MINDBLAST,false },
	{ D2S_MULTIPLESHOT,false },
	{ D2S_SMITE,true },
	{ D2S_TEETH,false },
	{ D2S_TORNADO,false },
	{ D2S_VOLCANO,false }
};

Aim::Aim()
{
}


Aim::~Aim()
{
}

void Aim::EnumeratePlayers()
{
	CArrayEx<PTARGETINFO, PTARGETINFO> TempInfo;
	INT nNewTarget = 0;

	if (Players.IsLocked)
		return;

	Players.Lock();

	for (INT i = 0; i < Players.GetSize(); i++)
		TempInfo.Add(Players[i]);

	Players.RemoveAll();

	for (RosterUnit* pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
	{
		bool Found = false;

		if (pUnit->dwUnitId == Me->dwUnitId)
			continue;

		if (pUnit->wLevel <= 8)
			continue;

		PTARGETINFO pInfo = new TARGETINFO;
		UnitAny* pPlayer = NULL;

		strcpy(pInfo->PlayerName, pUnit->szName);
		pInfo->UnitId = pUnit->dwUnitId;
		pInfo->Level = (BYTE)pUnit->wLevel;
		pInfo->ClassId = pUnit->dwClassId;
		pInfo->Life = (BYTE)pUnit->dwPartyLife;

		for (int i = 0; i < 6; i++)
			pInfo->Resists[i] = 0;

		for (int i = 0; i < TempInfo.GetSize(); i++)
		{
			if (TempInfo[i]->UnitId == pUnit->dwUnitId)
			{
				pInfo->Life = TempInfo[i]->Life;
				pInfo->X = TempInfo[i]->X;
				pInfo->Y = TempInfo[i]->Y;
				pInfo->HammerX = TempInfo[i]->HammerX;
				pInfo->HammerY = TempInfo[i]->HammerY;

				for (int j = 0; j < 6; j++)
					pInfo->Resists[j] = TempInfo[i]->Resists[j];
			}
		}

		pPlayer = (UnitAny*)GetUnit(pUnit->dwUnitId, UNIT_TYPE_PLAYER);
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

	for (int i = 0; i < TempInfo.GetSize(); i++)
		delete TempInfo[i];

	CurrentTarget = nNewTarget;
	Players.Unlock();

}

void Aim::Attack(DWORD x, DWORD y, bool Left, bool Shift, bool attackUnit, LPUNITANY Unit)
{
	if (!Units->CheckReady(false, true))
		return;

	if (attackUnit && !Unit)
		return;

	AttackStruct Attack;

	if (Shift)
		Attack.dwAttackType = Left ? 0xE5 : 0x66;
	else
		Attack.dwAttackType = Left ? 0xC5 : 0x46;

	Attack.lpPlayerUnit = D2CLIENT_GetPlayerUnit();
	Attack.dwTargetX = x;
	Attack.dwTargetY = y;
	Attack.lpTargetUnit = Unit;
	Stubs::D2CLIENT_Attack(&Attack, attackUnit);
}

void Aim::AttackTarget(bool Left)
{
	if (!Units->CheckReady(true, true))
		return;

	UnitAny* pTarget = D2CLIENT_FindServerSideUnit(CurrentTarget, UNIT_TYPE_PLAYER);

	if (!pTarget) return;

	for (auto& Atk : Attacks)
		if (Funcs->GetCurSkill(Left, Atk.SkillID))
			return Attack(NULL, NULL, Left, Atk.Shift, true, pTarget);

	for (INT i = 0; i < Players.GetSize(); i++)
		if (Players[i]->UnitId == pTarget->dwUnitId)
			return Attack(Players[i]->X, Players[i]->Y, Left, false, false, NULL);
}

void Aim::AttackNearest(bool Left)
{
	if (!Units->CheckReady(false, true))
		return;

	UnitAny* pTarget = Units->FindUnit();

	if (!pTarget)
		return;

	for (auto& Atk : Attacks)
		if (Funcs->GetCurSkill(Left, Atk.SkillID))
			return Attack(NULL, NULL, Left, Atk.Shift, true, pTarget);

	for (INT i = 0; i < Players.GetSize(); i++)
		if (Players[i]->UnitId == pTarget->dwUnitId)
			return Attack(Players[i]->X, Players[i]->Y, Left, false, false, NULL);
}

void Aim::TestKey()
{
	if (!Units->CheckReady(false, true))
		return;

	POINT ptDist = Units->FindUnitBS();

	if (D2MATH_GetDistance(Me, (WORD)ptDist.x, (WORD)ptDist.y) == 0)
	{
		if (Funcs->GetCurrentSkill(true) != D2S_BLESSEDHAMMER)
			SetSkill(D2S_BLESSEDHAMMER, true);
		if (Funcs->GetCurrentSkill(false) != D2S_CONCENTRATION)
			SetSkill(D2S_CONCENTRATION, false);

		Funcs->CastSpell(-3, 0, true);
	}
	else {
		if (Funcs->GetCurrentSkill(true) != D2S_TELEPORT)
			SetSkill(D2S_TELEPORT, true);
		AttackNearest(true);
	}
}

void Aim::SetBlind(WORD x, WORD y, WORD xHammer, WORD yHammer, DWORD UnitId)
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

void Aim::PrevTar()
{
	if (Players.IsEmpty())
		return;

	if (CurrentTarget == 0)
		CurrentTarget = Players.GetSize() - 1;
	else
		CurrentTarget--;
}

void Aim::NextTar()
{
	if (Players.IsEmpty())
		return;

	if (CurrentTarget == Players.GetSize() - 1)
		CurrentTarget = 0;
	else
		CurrentTarget++;
}