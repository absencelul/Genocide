#pragma once
class ItemPackets
{
public:
	ItemPackets();
	~ItemPackets();

	void DropItem(DWORD id);
	void PickItem(DWORD id);
	void PlaceItem(DWORD id, DWORD x, DWORD y, DWORD buffer);
};

