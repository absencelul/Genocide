#include "Hack.h"

bool STDCALL OnGamePacketSent(DWORD Length, DWORD _1, BYTE *Packet)
{
	switch (Packet[0])
	{
		//left attack packets
	case 0x05:
		//only set conc in this packet
		if (GetCurrentSkill(true) == D2S_BLESSEDHAMMER)
			SetSkill(D2S_CONCENTRATION, false);

		if (GetCurrentSkill(false) == D2S_CONCENTRATION && FindMercUnit(Me) && !GetUnitState(FindMercUnit(Me), AFFECT_FANATICISM))
			AllowSwap = true;
		break;

	case 0x06:
	case 0x07:
	case 0x08:
	case 0x09:
	case 0x0a:
		if (GetCurrentSkill(false) == D2S_CONCENTRATION && FindMercUnit(Me) && !GetUnitState(FindMercUnit(Me), AFFECT_FANATICISM))
			AllowSwap = true;
		break;

		//right attack packets

	case 0x0c:
	case 0x0d:
		if (GetCurrentSkill(false) == D2S_TELEPORT && FindMercUnit(Me) && !GetUnitState(FindMercUnit(Me), AFFECT_FANATICISM))
		{
			TeleSwap = true;
			AllowSwap = true;
		}
		break;

	case 0x13:
	{
		LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[5], UNIT_TYPE_OBJECT);

		if (PortalStates[1] && Unit && Unit->dwTxtFileNo == 0x3B && IsTown(Me))
			if (LifeMana(true) > 0 && LifeMana(true) <= Chickens[0])
				return FALSE;
	}
	break;
	}

	return D2NET_SendPacket(Length, _1, Packet);
}