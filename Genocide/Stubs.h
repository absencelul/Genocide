class Stubs
{
public:
	Stubs();
	~Stubs();
	static void OnGameLoopSTUB();
	static DWORD __stdcall ChickenSTUB(UnitAny* pUnit, DWORD dwStat, DWORD dwStat2);
	static void __fastcall D2CLIENT_Attack(AttackStruct* Attack, BOOL AttackingUnit);
	static void DrawAutomapSTUB();
	static void DrawInterfaceSTUB();
	static DWORD _fastcall DrawRectStub(RECT* pRect);
	static void GameFailToJoinSTUB();
	static void GamePacketReceived_STUB();
	static DWORD __fastcall GetUnitSTUB(DWORD dwUnitId, DWORD dwUnitType);
	static DWORD FASTCALL GetUnitNameSTUB(DWORD Unit);
	static void KeyHookSTUB();
	static void OnGameKeyDownSTUB();
	static void RefreshGameListSTUB();
	static DWORD FASTCALL GetUIVarSTUB(DWORD UI);
	static void ViewInventorySTUB1();
	static void ViewInventorySTUB2();
	static void ViewInventorySTUB3();
	//VOID ChatPacketReceivedSTUB();
	static void OnGameEndSTUB();
	static void InputSTUB();
	//void GameAttack_Intercept();
	static bool FASTCALL ChatText(const wchar_t *Text, int xPos, int yPos, DWORD Color, DWORD dwUnk);
	static AutomapLayer* __fastcall D2CLIENT_InitAutomapLayer_STUB(DWORD nLayerNo);
};
