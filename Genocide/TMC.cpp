#include "Hack.h"

TMC::TMC()
{
}

TMC::~TMC()
{
}

void __fastcall TMC::GameAfterAttack(LPUNITANY Unit, DWORD SkillID, DWORD SkillLevel, BOOL Unknown)
{
	if (!Unit)
		return;

	D2CLIENT_CreateAttack(Unit, SkillID, SkillLevel, Unknown);

	if (Me && Unit->dwType == UNIT_TYPE_PLAYER && Unit->dwUnitId == Me->dwUnitId)
	{
		auto skillsToRet = { D2S_LIGHTNING, D2S_CHAINLIGHTNING, D2S_CHARGE, D2S_LEAP, D2S_WHIRLWIND };

		for (auto i : skillsToRet)
			if (SkillID == i)
				return;

		Me->dwFrameRemain = 0;
	}
}