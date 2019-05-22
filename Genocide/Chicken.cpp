#include "Hack.h"

Chicken::Chicken()
{
}


Chicken::~Chicken()
{
}

//Checks Life // Mana and reacts accordingly.
void Chicken::Rescue()
{

	if (!Units->CheckReady(true, false))
		return;

	int Life = LifeMana(true), Mana = LifeMana(false);

	if (Life <= Chickens[1] && Chickens[1] > -1 || Mana <= Chickens[3] && Chickens[3] > -1)
		D2CLIENT_ExitGame();

	if (Life <= Chickens[0] && Chickens[0] > -1 || Mana <= Chickens[2] && Chickens[2] > -1)
		if ((GetTickCount64() - LastTP) >= 1000)
			if (Portal())
				Town = true;

	if (Life <= Potions[1] && Potions[1] > -1 || Mana <= Potions[3] && Potions[3] > -1)
		Potion::UsePot(2);

	if (Life <= Potions[0] && Potions[0] > -1)
		Potion::UsePot(0);

	if (Mana <= Potions[2] && Potions[2] > -1)
		Potion::UsePot(1);
}

//Checks if player is in town.
bool Chicken::IsTown(UnitAny* pUnit)
{
	if (!pUnit) return false;
	if (D2COMMON_GetTownLevel(pUnit->dwAct) == D2COMMON_GetLevelNoByRoom(pUnit->pPath->pRoom1)) return true;
	return false;
}

//Grabs Life or Mana.
INT Chicken::LifeMana(bool Life)
{
	INT LifePercent = Math::CalcPercent((GetUnitStat(D2CLIENT_GetPlayerUnit(), STAT_HP) >> 8), (GetUnitStat(D2CLIENT_GetPlayerUnit(), STAT_MAXHP) >> 8));

	INT ManaPercent = Math::CalcPercent((GetUnitStat(D2CLIENT_GetPlayerUnit(), STAT_MANA) >> 8), (GetUnitStat(D2CLIENT_GetPlayerUnit(), STAT_MAXMANA) >> 8));

	return Life ? LifePercent : ManaPercent;
}

//Checks for portal book.
bool Chicken::Portal()
{
	if (IsTown(D2CLIENT_GetPlayerUnit())) return false;

	if (!ClientReady) return false;

	LPUNITANY _Book = NULL;

	_Book = Funcs->FindItem(518, STORAGE_STASH);

	if (!_Book || GetUnitStat(_Book, STAT_AMMOQUANTITY) == 0)
		_Book = Funcs->FindItem(518, STORAGE_CUBE);

	if (!_Book || GetUnitStat(_Book, STAT_AMMOQUANTITY) == 0)
		_Book = Funcs->FindItem(518, STORAGE_INVENTORY);


	if (_Book)
		UseItem(_Book);
	else
		D2CLIENT_ExitGame();

	if (_Book)
		if (GetUnitStat(_Book, STAT_AMMOQUANTITY) <= 5)
			Funcs->Print(0, 4, (GetUnitStat(_Book, STAT_AMMOQUANTITY) - 1) >= 1 ? "%d Town portals left." : "Out of portals.", GetUnitStat(_Book, STAT_AMMOQUANTITY) - 1);

	LastTP = GetTickCount64();

	return true;
}