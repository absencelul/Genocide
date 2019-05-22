#include "DllMain.h"



Enchant::Enchant()
{
}


Enchant::~Enchant()
{
}


HANDLE Chant = 0;

DWORD WINAPI Enchant::ChantThread(LPVOID Parameter)
{

	if (!Funcs->GetSkill(D2S_ENCHANT))
	{
		Chant = 0;
		return 1;
	}

	LPUNITANY DemonLimb = D2COMMON_GetItemByBodyLoc(Me->pInventory, EQUIP_RIGHT_PRIMARY);

	if (DemonLimb->dwTxtFileNo != 214)
	{
		Chant = 0;
		return 1;
	}

	WORD OldSkill = Funcs->GetCurrentSkill(false);

	if (Funcs->GetCurrentSkill(false) != D2S_ENCHANT)
		SetChargedSkill(D2S_ENCHANT, false, DemonLimb->dwUnitId);

	while (Funcs->GetCurrentSkill(false) != D2S_ENCHANT)
		SleepEx(1, true);

	if (Funcs->GetCurrentSkill(false) == D2S_ENCHANT)
		while (Me->pInventory->pCursorItem)
			SleepEx(1, true);

	if (Funcs->GetCurrentSkill(false) == D2S_ENCHANT)
		UseSkillOnUnit(Units->FindMercUnit(Me), D2S_ENCHANT, false, true, true);

	while (!GetUnitState(Units->FindMercUnit(Me), AFFECT_ENCHANT))
		SleepEx(1, true);

	if (GetUnitState(Units->FindMercUnit(Me), AFFECT_ENCHANT))
	{
		Funcs->Print(0, 4, "Merc has been enchanted");
		//SetSkill(OldSkill, false);
	}

	Chant = 0;

	return 1;
}

void Enchant::enChant()
{

	if (!Units->FindMercUnit(Me) || GetUnitState(Units->FindMercUnit(Me), AFFECT_ENCHANT))
		return;

	if (Me->pInventory->pCursorItem)
		return;

	LPUNITANY DemonLimb = D2COMMON_GetItemByBodyLoc(Me->pInventory, EQUIP_RIGHT_PRIMARY);

	if (!DemonLimb)
		return;

	if (DemonLimb->dwTxtFileNo != 214)
		DemonLimb = NULL;

	if (DemonLimb)
		if (DemonLimb->pItemData->dwQuality == 7)
			if (!Chant)
				Chant = CreateThread(0, 0, ChantThread, 0, 0, 0);

}