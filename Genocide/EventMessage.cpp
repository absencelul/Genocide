#include "Hack.h"

DWORD EventMessageHandler(LPBYTE Packet, DWORD Length)
{
	switch (Packet[1])
	{
		// Player dropped from game
	case 0x00:
	{
		LPROSTERUNIT Unit = FindPartyByName((LPSTR)& Packet[8]);

		Print(0, 4, "[%s] %s(%s) [Level %d %s] dropped due to timeout.", TimeStamp(), Unit->szName, (LPSTR)& Packet[24], Unit->wLevel, PlayerClass(Unit->dwClassId, false));
		return false;
	}
	break;

	// Player joined game
	case 0x02:
	{
		LPROSTERUNIT Unit = FindPartyByName((LPSTR)& Packet[8]);

		if (!Unit || Unit->dwUnitId == Me->dwUnitId)
			return true;

		Print(0, 4, "[%s] %s(%s) [Level %d %s] joined our world.", TimeStamp(), Unit->szName, (LPSTR)& Packet[24], Unit->wLevel, PlayerClass(Unit->dwClassId, false));
		return false;
	}
	break;

	case 0x03:

		Print(0, 4, "[%s] %s(%s) [Level %s %s] [%s%% Life] left our world.", TimeStamp(), ChatName, (LPSTR)& Packet[24], ChatLevel, ChatClass, ChatLife);
		return false;

		break;

	case 0x06:
		if (!_stricmp((LPSTR)& Packet[24], Me->pPlayerData->szName))
		{
			LPROSTERUNIT Unit = FindPartyByName((LPSTR)& Packet[8]);

			if (!Unit)
				return true;

			Print(0, Red, "[%s] You killed '%s' a level %d %s.", TimeStamp(), Unit->szName, Unit->wLevel, PlayerClass(Unit->dwClassId, false));
			InputConsole("You killed %s [Level %d %s] total kills: %d, this game: %d", Unit->szName, Unit->wLevel, PlayerClass(Unit->dwClassId, false), ++killCount, ++killCountGame);

			if (UseAltScreen)
				D2CLIENT_SetUIVar(UI_ALTDOWN, 0, 0);

			TakeScreen = true;
			SStime = GetTickCount64();
			return false;
		}
		return true;
		break;
	}

	if (Packet[7])
	{
		LPROSTERUNIT Unit = FindPartyById(*(LPDWORD)& Packet[3]);

		sprintf(ChatClass, "%s", PlayerClass(Unit->dwClassId, false));
		sprintf(ChatClassShort, "%s", PlayerClass(Unit->dwClassId, true));
		sprintf(ChatName, "%s", Unit->szName);
		sprintf(ChatAccount, "%s", Unit->szName2);
		sprintf(ChatLevel, "%d", Unit->wLevel);
		sprintf(ChatLife, "%d", Unit->dwPartyLife);
	}

	switch (Packet[7])
	{
	case 0x01:
	{
		Print(1, 4, "[%s] %s permits you to loot his corpse.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x02:
	{
		Print(1, 4, "[%s] %s permits you to loot her corpse.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x03:
	{
		Print(1, 1, "[%s] %s has declared hostility towards you.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x04:
	{
		Print(1, 3, "[%s] %s is no longer hostile towards you.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x05:
	{
		Print(1, 2, "[%s] %s invites you to ally against the forces of evil.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x06:
	{
		Print(1, 9, "[%s] %s has canceled party invite.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x07:
	{
		Print(1, 2, "[%s] %s has joined your party to fight the forces of evil.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x08:
	{
		Print(1, 2, "[%s] You are now allied with %s.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x09:
	{
		Print(1, 9, "[%s] %s has left your party.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x0a:
	{
		Print(1, 4, "[%s] %s no longer allows you to access his corpse.", TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x0b:
	{
		Print(1, 4, "[%s] %s no longer allows you to access her corpse.", TimeStamp(), ChatName);
		return false;
	}
	break;
	}

	return true;
}