class Unit
{
public:
	Unit();
	virtual ~Unit();
	bool CheckReady(bool TownCheck, bool CheckCasting);
	LPUNITANY FindMercUnit(LPUNITANY pOwner);
	LPROSTERUNIT FindPartyById(DWORD dwUnitId);
	int GetRelation(UnitAny* unit);
	DWORD GetPvPFlags(DWORD dwPlayerId);
	char * PlayerClass(DWORD ClassId, bool Short);
	UnitAny* FindUnit();
	LPROSTERUNIT FindPartyByName(LPSTR szName);
	POINT FindUnitHammer();
	POINT FindUnitBS();
	LPLEVEL GetUnitLevel(LPUNITANY pUnit);
	//LPPRESETUNIT FindPresetUnit(DWORD dwClassId, DWORD dwType, LPROOM2 *pRoom);
};
