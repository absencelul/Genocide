
class MoveItems
{
public:
	MoveItems();
	~MoveItems();

	void MoveBufferToBuffer(DWORD oldBuffer, DWORD newBuffer);
	void MoveItemToBuffer(LPUNITANY Item, DWORD oldBuffer, DWORD newBuffer);
	void MoveItemsToBuffer(vector<LPUNITANY> Items, DWORD oldBuffer, DWORD newBuffer);
	void Move();

private:

	typedef struct ItemData
	{
		LPUNITANY Item;
		DWORD oldBuffer;
		DWORD newBuffer;
	} ITEMDATA, *PITEMDATA;

	POINT FindSpot(PITEMDATA Item, DWORD buffer);

	vector<PITEMDATA> Items;

};

