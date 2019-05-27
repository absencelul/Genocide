#include "Hack.h"

bool Portal(LPBYTE Packet, DWORD Length)
{
	if (Packet[0] == 0x60)
		if (Town && !TP->IsTown(Me))
		{
			LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[3], UNIT_TYPE_OBJECT);

			if (!_stricmp(Me->pPlayerData->szName, Unit->pObjectData->szOwner))
			{
				Interact(UNIT_TYPE_OBJECT, *(LPDWORD)&Packet[3]);
				Town = false;
			}
		}

	return true;
}

/*HANDLE portalThread = NULL;
DWORD ATPID = NULL;
bool Portal(LPBYTE Packet, DWORD Length)
{
	if (Town && !TP->IsTown(Me))
	{
		LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[3], UNIT_TYPE_OBJECT);
		if (!_stricmp(Me->pPlayerData->szName, Unit->pObjectData->szOwner))
		{
			ATPID = *(LPDWORD)&Packet[3];

			if (portalThread != NULL)
			{
				TerminateThread(portalThread, 0);
				portalThread = NULL;
			}
			portalThread = CreateThread(0, 0, EnterPortal, 0, 0, 0);
		}
	}
	return true;
}

DWORD WINAPI EnterPortal(LPVOID dwTimer)
{
	int count = 0;
	while (!TP->IsTown(Me) && ATPID != NULL) {
		Interact(ATPID, UNIT_TYPE_OBJECT);
		Sleep(Ping+50);
		count++;
	}
	Funcs->Print(false, 4, "%i", count);
	ATPID = NULL, Town = false;
	portalThread = 0;
	return 0;
}*/

/*#pragma pack(push, 1)
struct ChatPacket // max size(0x11A)
{
	BYTE px26;			//0x00
	BYTE ChatType;		//0x01
	BYTE MsgLang;		//0x02
	BYTE UnitType;		//0x03
	DWORD UnitId;		//0x04
	BYTE MsgColor;		//0x08
	BYTE MsgFlags;		//0x09
	char CharName[16];  //0x0A size 1-16
	char szMsg[256];	//0x1A -||- 1-256
};
#pragma pack(pop)*/

/*BOOL __fastcall OnMessage(BYTE* Packet, DWORD Length)
{
	ChatPacket *pMsg = (ChatPacket*)Packet;
	string szCharName = pMsg->CharName;
	int CharLen = strlen(pMsg->CharName);
	string szMsg = pMsg->CharName + CharLen + 1;
	//Funcs->Print(false, 4, "<%s> : %s", szCharName.c_str(), szMsg.c_str());
	return D2CLIENT_GameChat_I(Packet);
}*/

struct BaseSkill {
	WORD Skill;
	BYTE Level;
};

DWORD FASTCALL OnGamePacketReceived(LPBYTE Packet, DWORD Length)
{
	switch (Packet[0])
	{

	case 0x0D:
	{
		if (!Players.IsEmpty())
			for (auto i = 0; i < Players.GetSize(); i++)
				if (*(LPDWORD)&Packet[2] == Players[i]->UnitId)
				{
					if (Packet[12] > 100)
						Packet[12] = 100;

					Players[i]->Life = Packet[12];
				}

		LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[2], UNIT_TYPE_PLAYER);

		if (!Unit)
			return true;

		AA->SetBlind(*(LPWORD)&Packet[7], *(LPWORD)&Packet[9], NULL, NULL, *(LPDWORD)&Packet[2]);
	}
	break;
	case 0x0E:

		if (PortalStates[2])
		{
			LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[2], (DWORD)Packet[1]);

			if (Unit->dwType == UNIT_TYPE_OBJECT && Unit->dwTxtFileNo == 0x3B)
				return false;

			return true;
		}
	
	break;

	case 0x04:

		Revealz->RevealAutomap();

		break;

	case 0x15:

		if (*(LPDWORD)&Packet[2] == D2CLIENT_GetPlayerUnit()->dwUnitId)
		{
			Packet[10] = NULL;

			if (D2CLIENT_GetPlayerUnit()->dwMode == PLAYER_MODE_CAST)
				D2CLIENT_GetPlayerUnit()->dwFrameRemain = 0;

			if (D2CLIENT_GetPlayerUnit()->pPath->xPos == *(LPWORD)&Packet[6] && D2CLIENT_GetPlayerUnit()->pPath->yPos == *(LPWORD)&Packet[8])
				return true;
		}

		break;

	/*case 0x20:
		if (*(LPDWORD)&Packet[2] != D2CLIENT_GetPlayerUnit()->dwUnitId)
		{
			if (Packet[5] == STAT_FASTERCAST)
				if (Packet[6] < 500) Packet[6] = (BYTE)500;
		}
		break;*/

	case 0x26:
		if (_stricmp(Me->pPlayerData->szName, (LPSTR)&Packet[10]) && Spam((LPSTR)&Packet[strlen((LPSTR)&Packet[10]) + 11], (LPSTR)&Packet[10]))
			return false;
		//return OnMessage(Packet, Length);
	break;

	case 0x60: return Portal(Packet, Length);  break;

		//69   12   NPC State      69 [DWORD Unit Id] [BYTE State] [WORD X] [WORD Y] [BYTE Unit Life] [BYTE Unknown]
	case 0x69:

		if (RescueWhen[3])
			if (Units->FindMercUnit(D2CLIENT_GetPlayerUnit()) && Units->FindMercUnit(D2CLIENT_GetPlayerUnit())->dwUnitId == *(LPDWORD)&Packet[1])
				if (*(LPBYTE)&Packet[5] == 0x09 || *(LPBYTE)&Packet[5] == 0x08)
					if (TP->Portal())
					{
						Town = true;

						Funcs->InputConsole("RESCUE: Towned due to Mercenary Death!");
					}

		break;

	case 0x51: if (PortalStates[2] && Packet[6] == 0x3B) if (Packet[12] < 2) Packet[12] = 2; break;
	case 0x4c:
	{
		LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[2], UNIT_TYPE_PLAYER);

		if (!Unit)
			return true;

		//Sets PacketX/Y to blind coords on direct tele
		if (*(LPDWORD)&Packet[6] == D2S_TELEPORT)
			if (*(LPDWORD)&Packet[10] == Me->dwUnitId)
				AA->SetBlind(Me->pPath->xPos, Me->pPath->yPos, Unit->pPath->xPos, Unit->pPath->xPos, Unit->dwUnitId);

		if (*(LPDWORD)&Packet[6] != D2S_TELEPORT)
			if (*(LPDWORD)&Packet[10] == Me->dwUnitId)
				AA->SetBlind(Unit->pPath->xPos, Unit->pPath->yPos, Unit->pPath->xPos, Unit->pPath->xPos, Unit->dwUnitId);

		/*LPROSTERUNIT pRoster = Units->FindPartyById(Unit->dwUnitId);
		DWORD dwFlags = Units->GetPvPFlags(pRoster->dwUnitId);

		if (RescueWhen[2])
		{
			if (dwFlags & PVP_HOSTILED_BY_YOU || dwFlags & PVP_HOSTILED_YOU)
				if (TP->Portal())
				{
					Funcs->InputConsole("RESCUE: Towned due to [%s] attacking you", pRoster->szName);
					Town = true;
				}
		}*/
	}
	break;

	//Attack Cast X/Y Location
	case 0x4d:
	{
		//Checks if unit is npc
		if (Packet[1] == 0x01)
			return true;

		LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[2], UNIT_TYPE_PLAYER);

		if (!Unit)
			return true;

		//Sets PacketX/Y to cast location or Target location
		if (*(LPDWORD)&Packet[6] == D2S_TELEPORT || *(LPDWORD)&Packet[6] == D2S_CHARGE || *(LPDWORD)&Packet[6] == D2S_WHIRLWIND)
			AA->SetBlind(*(LPWORD)&Packet[11], *(LPWORD)&Packet[13], *(LPWORD)&Packet[11], *(LPWORD)&Packet[13], Unit->dwUnitId);
		else
			AA->SetBlind(Unit->pPath->xPos, Unit->pPath->yPos, *(LPWORD)&Packet[11], *(LPWORD)&Packet[13], Unit->dwUnitId);
	}
	break;

	case 0x5a: return eMSG->EventMessageHandler(Packet, Length); break;

	case 0x5c:
	{
		LPROSTERUNIT Roster = Units->FindPartyById(*(LPDWORD)&Packet[1]);

		if (!Roster)
			return true;

		sprintf(ChatClass, "%s", Units->PlayerClass(Roster->dwClassId, false));
		sprintf(ChatClassShort, "%s", Units->PlayerClass(Roster->dwClassId, true));
		sprintf(ChatName, "%s", Roster->szName);
		sprintf(ChatAccount, "%s", Roster->szName2);
		sprintf(ChatLevel, "%d", Roster->wLevel);
		sprintf(ChatLife, "%d", Roster->dwPartyLife > 100 ? 100 : Roster->dwPartyLife);

	}
	break;

	case 0xA7: if (PortalStates[0]) if (Packet[6] == AFFECT_JUST_PORTALED) return false; break;

	case 0xA8:
		if (*(LPBYTE)&Packet[1] == UNIT_TYPE_PLAYER && *(LPDWORD)&Packet[2] == Me->dwUnitId)
		{
			if (Packet[7] == AFFECT_BATTLEORDERS)
			{
				for (Skill* pSkill = Me->pInfo->pFirstSkill; pSkill; pSkill = pSkill->pNextSkill)
				{
					if (pSkill->pSkillInfo->wSkillId == D2S_BATTLEORDERS)
					{
						BoTimer = (D2COMMON_GetSkillLevel(Me, pSkill, true) * 10) + 20;
						BoTime = GetTickCount();
					}
				}
			}

			if (RescueWhenCursed[0])
			{
				if (Packet[7] == AFFECT_AMPLIFYDAMAGE)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Amplify Damage]");
						Town = true;
			}

			if (RescueWhenCursed[1])
			{
				if (Packet[7] == AFFECT_DIMVISION)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Dim Vision]");
						Town = true;
			}

			if (RescueWhenCursed[2])
			{
				if (Packet[7] == AFFECT_WEAKEN)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Weaken]");
						Town = true;
			}

			if (RescueWhenCursed[3])
			{
				if (Packet[7] == AFFECT_IRONMAIDEN)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Iron Maiden]");
						Town = true;
			}

			if (RescueWhenCursed[4])
			{
				if (Packet[7] == AFFECT_TERROR)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Terror]");
						Town = true;
			}

			if (RescueWhenCursed[5])
			{
				if (Packet[7] == AFFECT_CONFUSE)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Confuse]");
						Town = true;
			}

			if (RescueWhenCursed[6])
			{
				if (Packet[7] == AFFECT_LIFETAP)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Life Tap]");
						Town = true;
			}

			if (RescueWhenCursed[7])
			{
				if (Packet[7] == AFFECT_ATTRACT)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Attract]");
						Town = true;
			}

			if (RescueWhenCursed[8])
			{
				if (Packet[7] == AFFECT_DECREPIFY)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Decrepify]");
						Town = true;
			}

			if (RescueWhenCursed[9])
			{
				if (Packet[7] == AFFECT_LOWERRESIST)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Lower Resist]");
						Town = true;
			}
		}
		break;

	case 0xA9:

		if (*(LPBYTE)&Packet[1] == UNIT_TYPE_PLAYER && *(LPDWORD)&Packet[2] == Me->dwUnitId)
		{
			if (RescueWhenOut[0])
			{
				if (Packet[6] == AFFECT_BATTLEORDERS)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Battle Order] running out.");
						Town = true;
			}

			if (RescueWhenOut[1])
			{
				if (Packet[6] == AFFECT_BATTLECOMMAND)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Battle Command] running out.");
						Town = true;
			}

			if (RescueWhenOut[2])
			{
				if (Packet[6] == AFFECT_SHOUT)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Shout] running out.");
						Town = true;
			}

			if (RescueWhenOut[3])
			{
				if (Packet[6] == AFFECT_BONEARMOR)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Bone Armor] running out.");
						Town = true;
			}

			if (RescueWhenOut[4])
			{
				if (Packet[6] == AFFECT_CYCLONEARMOR)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Cyclone Armor] running out.");
						Town = true;
			}

			if (RescueWhenOut[5])
			{
				if (Packet[6] == AFFECT_FADE)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Fade] running out.");
						Town = true;
			}

			if (RescueWhenOut[6])
			{
				if (Packet[6] == AFFECT_HOLYSHIELD)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Holy Shield] running out.");
						Town = true;
			}

			if (RescueWhenOut[7])
			{
				if (Packet[6] == AFFECT_OAKSAGE)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Oak Sage] running out.");
						Town = true;
			}

			if (RescueWhenOut[8])
			{
				if (Packet[6] == AFFECT_CHILLINGARMOR)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Chilling Armor] running out.");
						Town = true;
			}

			if (RescueWhenOut[9])
			{
				if (Packet[6] == AFFECT_FROZENARMOR)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Frozen Armor] running out.");
						Town = true;
			}

			if (RescueWhenOut[10])
			{
				if (Packet[6] == AFFECT_ENERGYSHIELD)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Energy Shield] running out.");
						Town = true;
			}

			if (RescueWhenOut[11])
			{
				if (Packet[6] == AFFECT_ENCHANT)
					if (TP->Portal())
						Funcs->InputConsole("RESCUE: Towned due to [Enchant] running out.");
						Town = true;
			}
		}
		break;

	case 0x8C:

		if (RescueWhen[0] && *(LPWORD)&Packet[9] >= 8)
		{
			if (*(LPDWORD)&Packet[1] == Me->dwUnitId && !TP->IsTown(Me))
			{
				if (TP->Portal())
				{
					LPROSTERUNIT Unit = Units->FindPartyById(*(LPDWORD)&Packet[5]);
					Funcs->InputConsole("RESCUE: Towned due to [%s] hostiling you!", Unit->szName);
					Town = true;
				}
			}
		}

		break;
	}
	return true;
}