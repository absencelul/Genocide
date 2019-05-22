#include "Hack.h"

Potion::Potion()
{
}


Potion::~Potion()
{
}

//Uses potions. (Health: 0 Mana: 1 Rejuv: 2)
void Potion::UsePot(int Type) {

	if (!ClientReady || Me->pInventory->pCursorItem)
		return;

	vector<DWORD> PotCodes;

	switch (Type) {

	case 0:

		for (unsigned int i = 591; i > 586; i--)
			if (GetUnitState(Me, AFFECT_HEALTHPOT))
				return;
			else
				PotCodes.push_back(i);

		break;

	case 1:

		for (unsigned int i = 596; i > 591; i--)
			if (GetUnitState(Me, AFFECT_MANAPOT))
				return;
			else
				PotCodes.push_back(i);

		break;

	case 2:

		for (unsigned int i = 516; i > 513; i--)
			PotCodes.push_back(i);

		break;
	}

	LPUNITANY _Pot = NULL;

	for (unsigned int i = 0; i < PotCodes.size(); i++) {
		_Pot = Funcs->FindItem(PotCodes[i], STORAGE_STASH);

		if (!_Pot) _Pot = Funcs->FindItem(PotCodes[i], STORAGE_BELT);
		if (!_Pot) _Pot = Funcs->FindItem(PotCodes[i], STORAGE_INVENTORY);
		if (!_Pot) _Pot = Funcs->FindItem(PotCodes[i], STORAGE_CUBE);
		if (!_Pot) continue;

		UseItem(_Pot);
	}
}