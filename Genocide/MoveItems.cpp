#include "Hack.h"

HANDLE MoveThread = NULL;

MoveItems::MoveItems()
{

}


MoveItems::~MoveItems()
{
	
}


DWORD __stdcall StartThread(LPVOID params)
{
	if (!params)
		return 0;

	MoveItems * items = (MoveItems*)params;

	items->Move();

	return 1;
}

void MoveItems::MoveBufferToBuffer(DWORD oldBuffer, DWORD newBuffer)
{

	LPUNITANY Item = Me->pInventory->pFirstItem;

	if (!Item || MoveThread)
		return;

	do
	{
		if (Item->dwType == UNIT_TYPE_ITEM && Item->pItemData->ItemLocation == oldBuffer)
		{
			PITEMDATA item = new ITEMDATA();
			item->Item = Item;
			item->oldBuffer = oldBuffer;
			item->newBuffer = newBuffer;
			Items.push_back(item);
		}

	} while ((Item = Item->pItemData->pNextInvItem));

	MoveThread = CreateThread(NULL, NULL, StartThread, this, NULL, NULL);
}

void MoveItems::MoveItemToBuffer(LPUNITANY Item, DWORD oldBuffer, DWORD newBuffer)
{
	if (MoveThread)
		return;

	PITEMDATA item = new ITEMDATA();
	item->Item = Item;
	item->oldBuffer = oldBuffer;
	item->newBuffer = newBuffer;
	Items.push_back(item);

	MoveThread = CreateThread(NULL, NULL, StartThread, this, NULL, NULL);
}

void MoveItems::MoveItemsToBuffer(vector<LPUNITANY> items, DWORD oldBuffer, DWORD newBuffer)
{
	if (MoveThread)
		return;

	for each(LPUNITANY Item in items)
	{
		PITEMDATA item = new ITEMDATA();
		item->Item = Item;
		item->oldBuffer = oldBuffer;
		item->newBuffer = newBuffer;
		Items.push_back(item);
	}

	MoveThread = CreateThread(NULL, NULL, StartThread, this, NULL, NULL);
}

void MoveItems::Move()
{
	ItemPackets item;

	for each(PITEMDATA Item in Items)
	{
		POINT newLocation;

		while (!Me->pInventory->pCursorItem)
		{
			if (Item->newBuffer == 255)
				item.PickItem(Item->Item->dwUnitId);
			else
			{
				newLocation = FindSpot(Item, Item->newBuffer);

				if (newLocation.x == -1 || newLocation.y == -1)
					goto end;

				item.PickItem(Item->Item->dwUnitId);
			}

			Sleep(30);
		}

		while (Me->pInventory->pCursorItem)
		{
			if (Item->newBuffer == 255)
				item.DropItem(Item->Item->dwUnitId);
			else
				item.PlaceItem(Item->Item->dwUnitId, newLocation.x, newLocation.y, Item->newBuffer);

			Sleep(30);
		}
	}

end:
	Items.clear();
	MoveThread = NULL;
	TerminateThread(GetCurrentThread(), 0);
}


POINT MoveItems::FindSpot(PITEMDATA Item, DWORD buffer)
{
	if (!Item->Item)
		return { -1, -1 };

	int w = 0, h = 0;

	switch (buffer)
	{
		//Inventory
	case 0:
		w = 10, h = 4;
		break;
		//Cube
	case 3:
		w = 3, h = 4;
		break;
		//Stash
	case 4:
		w = 6, h = 8;
		break;
	}

	int inventory[8][10];

	for (int i = 0; i < 10; i++)
		for (int z = 0; z < 8; z++)
			inventory[z][i] = 0;

	LPUNITANY tempItem = Me->pInventory->pFirstItem;

	if (!tempItem)
		return { -1, -1 };

	do
	{
		if (tempItem->dwType == 4)
		{
			if (tempItem->pItemData->ItemLocation == buffer)
			{
				LPITEMTXT nItem = D2COMMON_GetItemText(tempItem->dwTxtFileNo);

				for (int x = tempItem->pItemPath->dwPosX; x < (nItem->xSize + tempItem->pItemPath->dwPosX); x++)
				{
					for (int y = tempItem->pItemPath->dwPosY; y < (tempItem->pItemPath->dwPosY + nItem->ySize); y++)
					{
						inventory[y][x] = 1;
					}
				}
			}
		}
	} while ((tempItem = tempItem->pItemData->pNextInvItem));

	LPITEMTXT nItem = D2COMMON_GetItemText(Item->Item->dwTxtFileNo);

	POINT PlaceItem = { 0 ,0 };

	for (int a = 0; a < w; a++) //width
	{
		for (int b = 0; b < h; b++) //height
		{
			if (nItem->ySize + b > h)
				break;

			if (inventory[b][a] == 1)
				continue;

			bool valid = true;
			for (int c = 0; c < nItem->ySize; c++)
			{

				if (nItem->xSize == 1)
				{
					if (inventory[(b + c)][a] == 1)
					{
						valid = false;
						break;
					}
				}
				else
				{
					if (inventory[(b + c)][a] == 1 || inventory[(b + c)][a + 1] == 1)
					{
						valid = false;
						break;
					}
				}
			}

			if (valid == true)
			{
				PlaceItem.x = (long)a;
				PlaceItem.y = (long)b;
				goto fin;
			}
		}
	}

	return { -1, -1 };

fin:
	delete nItem;
	delete[] inventory;
	delete tempItem;

	return PlaceItem;
}