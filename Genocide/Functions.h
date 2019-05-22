class Functions
{
public:
	Functions();
	virtual ~Functions();
	//void Testing();
	LPUNITANY FASTCALL FindItem(DWORD dwCode, INT nLoc);
	int GetItemLocation(LPUNITANY pItem);
	bool GetCurSkill(bool left, WORD wSkillId);
	WORD GetCurrentSkill(bool Left);
	char * Difficulty();
	bool GetSkill(WORD wSkillId);
	void Print(bool Bottom, int Color, char * Format, ...);
	void PrintConsole(LPSTR lpMessage, ...);
	INT RemoveColorSpecs(LPWSTR lpwsz);
	DWORD UseItemIds(INT nLoc);
	void CastSpell(WORD x, WORD y, bool MySpot);
	LPSTR AddCommas(DOUBLE f, INT places);
	void ScreenToAutoMap(LPPOINT Point);
	void ScreenToAutomap(POINT* ptPos, int x, int y);
	//string GetItemCode(UnitAny* ptItem);
	CHAR* SysTime();
	CHAR* TimeStamp();
};

LPUNITANY GetPlayer();
LPUNITANY ViewInventoryPatch1();