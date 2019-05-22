#include "DllMain.h"

Invite::Invite()
{
}


Invite::~Invite()
{
}

/*DWORD WINAPI Invite::InviteThread(LPVOID Parameter)
{
	while (bRunningThreads)
	{
		Sleep(10);

		if (ClientReady)
		{
			if (InviteQueue.IsLocked)
				continue;

			InviteQueue.Lock();

			while (!InviteQueue.empty())
			{
				if (InviteQueue[0]->bInvite)
					if (!Helpers::IsPlayerInAParty(Helpers::GetRosterUnitFromId(InviteQueue[0]->dwPlayerId)))
						if (Helpers::GetRosterUnitFromId(InviteQueue[0]->dwPlayerId)->dwPartyFlags != 2)
							Helpers::InviteAction(INVITE_PLAYER, InviteQueue[0]->dwPlayerId);


				if (InviteQueue[0]->bLoot)
					if ((*p_D2LAUNCH_BnData)->nCharFlags & PLAYER_TYPE_HARDCORE)
						Helpers::ButtonAction(LOOT_BUTTON, TOGGLE_ON, InviteQueue[0]->dwPlayerId);

				delete InviteQueue[0];
				InviteQueue.erase(InviteQueue.begin());

				Sleep(1);
			}

			InviteQueue.Unlock();

			if (!JoinPartyOnce && AutoInvite)
			{
				RosterUnit* pRosterMe = Helpers::GetRosterUnitFromId(Me->dwUnitId);

				if (pRosterMe)
				{
					if (pRosterMe->wPartyId != NO_PARTY)
					{
						JoinPartyOnce = true;
					}
					else
					{
						WORD wAcceptPartyId = NULL;
						DWORD dwAcceptId = NULL;

						for (RosterUnit* pRoster = *p_D2CLIENT_PlayerUnitList; pRoster; pRoster = pRoster->pNext)
						{
							if (pRoster->dwUnitId == Me->dwUnitId)
								continue;

							if (pRoster->dwPartyFlags == 2)
							{
								if (wAcceptPartyId == NULL || pRoster->wPartyId != wAcceptPartyId && pRoster->wPartyId != NO_PARTY)
								{
									wAcceptPartyId = pRoster->wPartyId;
									dwAcceptId = pRoster->dwUnitId;
								}
							}
						}

						if (dwAcceptId != NULL)
						{
							Helpers::InviteAction(ACCEPT_INVITE, dwAcceptId);
							JoinPartyOnce = true;
						}
					}
				}
			}
		}
	}
}*/