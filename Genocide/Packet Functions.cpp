#include "Hack.h"
//Interacts with assigned object.
void Interact(DWORD UnitType, DWORD UnitId)
{
#pragma pack(push,1)
	struct p0x13
	{
		BYTE Header;
		DWORD UnitType;
		DWORD UnitId;
	};
#pragma pack(pop)

	p0x13 Packet = { 0x13, UnitType, UnitId };
	D2NET_SendPacket(sizeof(Packet), 1, (BYTE*)&Packet);
}

//Interacts with assigned object.
/*bool Interact(DWORD ID, DWORD Type)
{

	if (!ClientReady)
		return false;

	LPBYTE Packet = new BYTE[9];
	Packet[0] = 0x13;
	*(LPDWORD)&Packet[1] = Type;
	*(LPDWORD)&Packet[5] = ID;
	D2NET_SendPacket(9, 1, Packet);
	delete[] Packet;

	return true;
}*/

//Takes WP
void TakeWP(DWORD WpID, BYTE Destination)
{
	LPBYTE Packet = new BYTE[9];
	Packet[0] = 0x49;
	*(LPDWORD)&Packet[1] = WpID;
	*(LPBYTE)Packet[5] = Destination;
	D2NET_SendPacket(9, 1, Packet);
	delete[] Packet;
}

//Resynch player
void ResynchPlayer(DWORD ID, INT Type)
{
	if (!GetUnit(ID, Type))
		return;

	LPBYTE Packet = new BYTE[9];
	Packet[0] = 0x4B;
	*(LPDWORD)&Packet[1] = Type;
	*(LPDWORD)&Packet[5] = ID;
	D2NET_SendPacket(9, 1, Packet);
	delete[] Packet;
}

bool SetSkill(WORD wSkillId, bool bLeft)
{
	if (!D2CLIENT_GetPlayerUnit())
		return 0;

	BYTE Packet[9] = { 0x3c, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF };
	*(WORD*)&Packet[1] = wSkillId;
	*(BYTE*)&Packet[4] = bLeft ? 0x80 : 0x00;
	D2NET_SendPacket(sizeof(Packet), 0, Packet);

	return true;
}

//Sets skill left//right
/*BOOL SetSkill(WORD Skill, bool Left)
{
	BYTE Packet[9];
	Packet[0] = 0x3C;
	*(LPWORD)&Packet[1] = Skill;
	Packet[3] = NULL;
	Packet[4] = Left ? 0x80 : 0x00;
	*(LPDWORD)&Packet[5] = 0xFFFFFFFF;
	D2NET_SendPacket(9, 1, Packet);
	return true;
}*/

void HostilePlayer(DWORD dwUnitId)
{
	BYTE Packet[7];
	Packet[0] = 0x5D;
	*(WORD*)&Packet[1] = 0x104;
	*(DWORD*)&Packet[3] = dwUnitId;
	D2NET_SendPacket(7, 1, Packet);
	return;
}

//Tele walk
void Teleportz(DWORD X, DWORD Y)
{
	if (!ClientReady)
		return;

	LPBYTE Packet = new BYTE[5];
	Packet[0] = 0x5F;
	*(LPDWORD)&Packet[1] = X;
	*(LPDWORD)&Packet[3] = Y;
	D2NET_SendPacket(5, 1, Packet);
	delete[] Packet;

	LPBYTE Update = new BYTE[9];
	Update[0] = 0x4B;
	*(LPDWORD)&Update[1] = Me->dwType;
	*(LPDWORD)&Update[5] = Me->dwUnitId;
	D2NET_SendPacket(9, 1, Update);
	delete[] Update;
	return;
}

//Switches weapon side
void SwitchWeapon()
{
	LPBYTE Packet = new BYTE[1];
	Packet[0] = 0x60;
	D2NET_SendPacket(1, 1, Packet);
	delete[] Packet;
}

//Sets Charged skill left//right
bool SetChargedSkill(WORD wSkillId, bool bLeft, DWORD dwItemId)
{
	if (!ClientReady)
		return false;

	if (!GetSkill(wSkillId))
		return false;

	BYTE Packet[9];

	Packet[0] = 0x3C;
	*(WORD*)&Packet[1] = wSkillId;
	Packet[3] = 0;
	Packet[4] = (bLeft) ? 0x80 : 0;
	*(DWORD*)&Packet[5] = dwItemId;

	D2NET_SendPacket(9, 1, Packet);
	return true;
}

//Uses selected item. i.e Portals,Pots
void UseItem(LPUNITANY Unit)
{
	if (!Unit)
		return;

	INT Location = GetItemLocation(Unit);

	if (Location == STORAGE_INVENTORY || Location == STORAGE_BELT) {
		LPBYTE Packet = new BYTE[13];
		Packet[0] = (Location == STORAGE_INVENTORY) ? 0x20 : 0x26;
		*(LPDWORD)&Packet[1] = Unit->dwUnitId;
		*(LPDWORD)&Packet[5] = (Location == STORAGE_INVENTORY) ? Me->pPath->xPos : NULL;
		*(LPDWORD)&Packet[9] = (Location == STORAGE_INVENTORY) ? Me->pPath->yPos : NULL;
		D2NET_SendPacket(13, 1, Packet);
		delete[] Packet;
	}

	if (Location == STORAGE_STASH || Location == STORAGE_CUBE) {
		LPBYTE Packet = new BYTE[9];
		Packet[0] = 0x27;
		*(LPDWORD)&Packet[1] = UseItemIds(Location);
		*(LPDWORD)&Packet[5] = Unit->dwUnitId;
		D2NET_SendPacket(9, 1, Packet);
		delete[] Packet;
	}
}

//Uses skill on a unit
void UseSkillOnUnit(LPUNITANY Unit, WORD Skill, bool Left, bool Shift, bool Animation)
{
	if (!CheckReady(false, true))
		return;

	if (!GetSkill(Skill))
		return;

	LPBYTE Packet = new BYTE[9];
	Packet[0] = Shift ? (Left ? 0x0A : 0x11) : (Left ? 0x06 : 0x0D);
	*(LPDWORD)&Packet[1] = Unit->dwType;
	*(LPDWORD)&Packet[5] = Unit->dwUnitId;
	D2NET_SendPacket(9, 1, Packet);
	delete[] Packet;

	if (Animation)
	{
		LPBYTE Packet = new BYTE[16];
		Packet[0] = 0x4C;
		*(LPDWORD)&Packet[1] = Me->dwType;
		*(LPDWORD)&Packet[2] = Me->dwUnitId;
		*(LPWORD)&Packet[6] = Skill;
		*(LPDWORD)&Packet[9] = Unit->dwType;
		*(LPDWORD)&Packet[10] = Unit->dwUnitId;
		D2NET_ReceivePacket(Packet, 16);
		delete[] Packet;
	}
}