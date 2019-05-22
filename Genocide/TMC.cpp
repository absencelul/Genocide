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
		int skillsToRet[7] = { 49, 53, 107, 132, 151 };

		for (int i = 0; i < ArraySize(skillsToRet); i++)
			if (SkillID == skillsToRet[i])
				return;

		Me->dwFrameRemain = 0;
	}
}