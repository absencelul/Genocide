#include "Hack.h"

EventMessage::EventMessage()
{
}


EventMessage::~EventMessage()
{
}

DWORD EventMessage::EventMessageHandler(LPBYTE Packet, DWORD Length)
{
	switch (Packet[1]) {

		// Player dropped from game
	case 0x00:
	{
		LPROSTERUNIT Unit = Units->FindPartyByName((LPSTR)&Packet[8]);

		Funcs->Print(0, 4, "[%s] %s(%s) [Level %d %s] dropped due to timeout.", Funcs->TimeStamp(), Unit->szName, (LPSTR)&Packet[24], Unit->wLevel, Units->PlayerClass(Unit->dwClassId, false));
		return false;
	}
		break;

		// Player joined game
	case 0x02:
	{
		LPROSTERUNIT Unit = Units->FindPartyByName((LPSTR)&Packet[8]);

		if (!Unit || Unit->dwUnitId == Me->dwUnitId)
			return true;

		Funcs->Print(0, 4, "[%s] %s(%s) [Level %d %s] joined our world.", Funcs->TimeStamp(), Unit->szName, (LPSTR)&Packet[24], Unit->wLevel, Units->PlayerClass(Unit->dwClassId, false));
		return false;
	}
		break;

	case 0x03:

		Funcs->Print(0, 4, "[%s] %s(%s) [Level %s %s] [%s%% Life] left our world.", Funcs->TimeStamp(), ChatName, (LPSTR)&Packet[24], ChatLevel, ChatClass, ChatLife);
		return false;

		break;

	case 0x06:
		if (!_stricmp((LPSTR)&Packet[24], Me->pPlayerData->szName))
		{
			LPROSTERUNIT Unit = Units->FindPartyByName((LPSTR)&Packet[8]);

			if (!Unit)
				return true;

			Funcs->Print(0, 5, "[%s] You killed '%s' [Level %d %s].", Funcs->TimeStamp(), Unit->szName, Unit->wLevel, Units->PlayerClass(Unit->dwClassId, false));
			Funcs->InputConsole("You killed %s [Level %d %s]", Unit->szName, Unit->wLevel, Units->PlayerClass(Unit->dwClassId, false));
			Funcs->InputConsole("Total kill count: %d, kills this game: %d", ++killCount, ++killCountGame);

			if (UseAltScreen)
				D2CLIENT_SetUIVar(UI_ALTDOWN, 0, 0);

			TakeScreen = true;
			SStime = GetTickCount();
			return false;
		}
		return true;
	break;
	}

	if (Packet[7])
	{
		LPROSTERUNIT Unit = Units->FindPartyById(*(LPDWORD)&Packet[3]);

		sprintf(ChatClass, "%s", Units->PlayerClass(Unit->dwClassId, false));
		sprintf(ChatClassShort, "%s", Units->PlayerClass(Unit->dwClassId, true));
		sprintf(ChatName, "%s", Unit->szName);
		sprintf(ChatAccount, "%s", Unit->szName2);
		sprintf(ChatLevel, "%d", Unit->wLevel);
		sprintf(ChatLife, "%d", Unit->dwPartyLife);
	}

	switch (Packet[7])
	{
	case 0x01:
	{
		Funcs->Print(1, 4, "[%s] %s permits you to loot his corpse.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x02:
	{
		Funcs->Print(1, 4, "[%s] %s permits you to loot her corpse.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x03:
	{
		Funcs->Print(1, 1, "[%s] %s has declared hostility towards you.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x04:
	{
		Funcs->Print(1, 3, "[%s] %s is no longer hostile towards you.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x05:
	{
		Funcs->Print(1, 2, "[%s] %s invites you to ally against the forces of evil.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x06:
	{
		Funcs->Print(1, 9, "[%s] %s has canceled party invite.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x07:
	{
		Funcs->Print(1, 2, "[%s] %s has joined your party to fight the forces of evil.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x08:
	{
		Funcs->Print(1, 2, "[%s] You are now allied with %s.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x09:
	{
		Funcs->Print(1, 9, "[%s] %s has left your party.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x0a:
	{
		Funcs->Print(1, 4, "[%s] %s no longer allows you to access his corpse.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	case 0x0b:
	{
		Funcs->Print(1, 4, "[%s] %s no longer allows you to access her corpse.", Funcs->TimeStamp(), ChatName);
		return false;
	}
	break;

	}

	return true;
}