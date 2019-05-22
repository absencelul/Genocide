#include "Hack.h"



ItemPackets::ItemPackets()
{
}


ItemPackets::~ItemPackets()
{
}


void ItemPackets::DropItem(DWORD id)
{
	LPBYTE Packet = new BYTE[5];
	Packet[0] = 0x17;
	*(LPDWORD)&Packet[1] = id;
	D2NET_SendPacket(5, 1, Packet);
	delete[] Packet;
}

void ItemPackets::PickItem(DWORD id)
{
	LPBYTE Packet = new BYTE[5];
	Packet[0] = 0x19;
	*(LPDWORD)&Packet[1] = id;
	D2NET_SendPacket(5, 1, Packet);
	delete[] Packet;
}

void ItemPackets::PlaceItem(DWORD id, DWORD x, DWORD y, DWORD buffer)
{
	LPBYTE Packet = new BYTE[17];
	Packet[0] = 0x18;
	*(LPDWORD)&Packet[1] = id;
	*(LPDWORD)&Packet[5] = x;
	*(LPDWORD)&Packet[9] = y;
	*(LPDWORD)&Packet[13] = buffer;
	D2NET_SendPacket(17, 1, Packet);
	delete[] Packet;
}
