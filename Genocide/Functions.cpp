#include "Hack.h"

Functions::Functions()
{
}


Functions::~Functions()
{
}

/*void Functions::Testing()
{
	UnitAny* Ring1 = 0;
	UnitAny* Ring2 = 0;
	DWORD tmp1 = 0;

	for (UnitAny* pItem = Me->pInventory->pFirstItem; pItem; pItem = pItem->pItemData->pNextInvItem)
	{
		if (GetItemLocation(pItem) == STORAGE_CUBE)
		{
			string szTemp = GetItemCode(pItem);

			if (szTemp.empty()) continue;
			if (szTemp.compare(0, 3, "rin") == 0 && pItem->pItemData->dwQuality2 == 268)
			{
				if (!Ring1)
					Ring1 = pItem;
				else if (Ring1 && !Ring2)
					Ring2 = pItem;
				continue;
			}
			if (szTemp.compare(0, 3, "rin") == 0 && pItem->pItemData->dwQuality2 == 122)
			{
				if (!Ring1)
					Ring1 = pItem;
				else if (Ring1 && !Ring2)
					Ring2 = pItem;
				continue;
			}
		}
	}
}*/

//add commas
LPSTR Functions::AddCommas(DOUBLE f, INT places)
{
	static CHAR tmp[40];
	LPSTR s;
	INT i = 0;

	if ((places < 0) || (places > 38))
	{
		tmp[0] = 0;
		return tmp;
	}

	sprintf_s(tmp, "%#.*f", places, f);
	s = strchr(tmp, '.');
	if (!places)
		*s = '\0';

	while (--s > tmp)
	{
		if (i++ == 2)
		{
			if (s[-1] == '-') break;
			i = 0;
			memmove(s + 1, s, strlen(s) + 1);
			*s = ',';
		}
	}

	return tmp;
}

//Searches and returns Item as unit.
LPUNITANY FASTCALL Functions::FindItem(DWORD dwCode, INT nLoc)
{
	LPUNITANY Item = Me->pInventory->pFirstItem;

	if (!Item)
		return false;

	do
	{
		if (Item->dwType == UNIT_TYPE_ITEM)
		{
			if (Item->dwTxtFileNo == dwCode)
			{
				if (Item->dwTxtFileNo == 518 && GetUnitStat(Item, STAT_AMMOQUANTITY) == 0)
					continue;

				if (GetItemLocation(Item) == nLoc)
					return Item;
			}
		}
	} while ((Item = Item->pItemData->pNextInvItem));

	return false;
}

//Searches for item and returns location. i.e Stash Cube etc.
int Functions::GetItemLocation(LPUNITANY pItem)
{
	if (!pItem || !pItem->pItemData)
		return -1;

	switch (pItem->pItemData->ItemLocation)
	{
	case STORAGE_INVENTORY:
		return STORAGE_INVENTORY;

	case STORAGE_TRADE:
		if (pItem->pItemData->NodePage == NODEPAGE_STORAGE)
			return STORAGE_TRADE;

	case STORAGE_CUBE:
		return STORAGE_CUBE;

	case STORAGE_STASH:
		return STORAGE_STASH;

	case STORAGE_NULL:
		switch (pItem->pItemData->NodePage)
		{
		case NODEPAGE_EQUIP:
			return STORAGE_EQUIP;

		case NODEPAGE_BELTSLOTS:
			return STORAGE_BELT;
		}
	}

	return STORAGE_NULL;
}

bool Functions::GetCurSkill(bool left, WORD wSkillId)
{
	if (left)
		return (Me->pInfo->pLeftSkill->pSkillInfo->wSkillId == wSkillId);
	else
		return (Me->pInfo->pRightSkill->pSkillInfo->wSkillId == wSkillId);
}

//Get current selected skill left/right
WORD Functions::GetCurrentSkill(bool Left)
{
	if (Left)
		return Me->pInfo->pLeftSkill->pSkillInfo->wSkillId;
	else
		return Me->pInfo->pRightSkill->pSkillInfo->wSkillId;
}

void Functions::CastSpell(WORD x, WORD y, bool MySpot)
{
	if (!Units->CheckReady(true, true))
		return;

	AttackStruct Attack;

	Attack.dwAttackType = 0xE5;

	Attack.dwTargetX = MySpot ? Me->pPath->xPos + x : x;
	Attack.dwTargetY = MySpot ? Me->pPath->yPos + y : y;

	Attack.lpPlayerUnit = Me;

	Attack.lpTargetUnit = NULL;

	Stubs::D2CLIENT_Attack(&Attack, false);
}

char * Functions::Difficulty()
{
	switch (D2CLIENT_GetDifficulty())
	{
	case 0:
		return "Normal";
		break;
	case 1:
		return "Nightmare";
		break;
	case 2:
		return "Hell";
		break;
	}
	return "NULL";
}

//Checks to see if you have skill
bool Functions::GetSkill(WORD wSkillId)
{
	if (!Me)
		return 0;

	for (Skill* pSkill = Me->pInfo->pFirstSkill; pSkill; pSkill = pSkill->pNextSkill)
		if (pSkill->pSkillInfo->wSkillId == wSkillId)
			return 1;

	return 0;
}

//Prints text to screen.
void Functions::Print(bool Bottom, int Color, char * Format, ...)
{
	if (strlen(Format) <= NULL)
		return;

	char szString[152] = { 0 };
	va_list vaArgs;
	va_start(vaArgs, Format);
	vsprintf_s(szString, 152, Format, vaArgs);
	va_end(vaArgs);
	wchar_t Buffer[0x130];
	MultiByteToWideChar(0, 1, szString, 152, Buffer, 304);

	if (Bottom)
		D2CLIENT_PrintPartyString(Buffer, Color);
	else
		D2CLIENT_PrintGameString(Buffer, Color);
}

void Functions::InputConsole(std::string msg, ...)
{
	if ((int)ConsoleMessages.size() >= 3)
		ConsoleMessages.erase(ConsoleMessages.begin());

	char szBuffer[4096];
	va_list arg;
	va_start(arg, msg);
	vsprintf_s(szBuffer, 4096, msg.c_str(), arg);
	va_end(arg);

	LPCONSOLEMESSAGE pConsoleMessage = new CONSOLEMESSAGE;
	pConsoleMessage->Timer = GetTickCount64();
	pConsoleMessage->ConsoleString = szBuffer;
	ConsoleMessages.push_back(pConsoleMessage);
}

//remove yc within string
INT Functions::RemoveColorSpecs(LPWSTR lpwsz)
{
	if (lpwsz == NULL)
		return 0;

	CONST INT LEN = (INT)wcslen(lpwsz);
	LPWSTR p = new WCHAR[LEN + 1];
	memset(p, 0, sizeof(WCHAR) * (LEN + 1));

	INT nIndex = 0;
	for (INT i = 0; i < LEN; i++)
	{
		if (lpwsz[i] == 0xF8F5 || lpwsz[i] == 0xFF)
			i += 2;
		else
			p[nIndex++] = lpwsz[i];
	}

	wcscpy(lpwsz, p);
	delete[] p; //memory leak again idiots.
	return (INT)wcslen(lpwsz);
}

//Casts screen to automap
void Functions::ScreenToAutoMap(LPPOINT Point)
{
	POINT Mouse = { (long)(*p_D2CLIENT_MouseX), (long)(*p_D2CLIENT_MouseY) };
	POINT MouseOffset = { (*p_D2CLIENT_MouseOffsetX), (*p_D2CLIENT_MouseOffsetY) };

	Mouse.x += MouseOffset.x;
	Mouse.y += MouseOffset.y;

	D2COMMON_AbsScreenToMap(&Mouse.x, &Mouse.y);

	Point->x = Mouse.x;
	Point->y = Mouse.y;
}

LPUNITANY GetPlayer()
{
	if (!ViewingUnit)
		return Me;

	return ViewingUnit;
}

LPUNITANY ViewInventoryPatch1()
{
	return GetPlayer();
}

//Casts screen to automap
void Functions::ScreenToAutomap(POINT* ptPos, int x, int y)
{
	x *= 32; y *= 32;
	ptPos->x = ((x - y) / 2 / (*(INT*)p_D2CLIENT_Divisor)) - (*p_D2CLIENT_Offset).x + 8;
	ptPos->y = ((x + y) / 4 / (*(INT*)p_D2CLIENT_Divisor)) - (*p_D2CLIENT_Offset).y - 8;
	if (D2CLIENT_GetAutomapSize())
	{
		--ptPos->x;
		ptPos->y += 5;
	}
}

//Searches for specified item id and returns it.
DWORD Functions::UseItemIds(INT nLoc)
{
	if (Me->pInventory->dwLeftItemUid)
		return Me->pInventory->dwLeftItemUid;

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item->dwType == UNIT_TYPE_ITEM && Item->dwUnitId)
		{
			if (Item->pInventory)
			{
				if (GetItemLocation(Item) != nLoc)
					return Item->dwUnitId;
			}
		}
	}

	if (Me->pInventory->pLastItem)
		return Me->pInventory->pLastItem->dwUnitId;

	return NULL;
}

/*string Functions::GetItemCode(UnitAny* ptItem)
{
	if (!ptItem)
		return 0;

	ItemTxt* pTxt = D2COMMON_GetItemText(ptItem->dwTxtFileNo);
	return pTxt->szCode;
}*/

//System Time
CHAR* Functions::SysTime()
{
	SYSTEMTIME Time;
	GetLocalTime(&Time);

	sprintf_s(LocalTime, "%.2d/%.2d/%.2d - %.2d:%.2d:%.2d", Time.wMonth, Time.wDay, Time.wYear, Time.wHour, Time.wMinute, Time.wSecond);

	return LocalTime;
}

//Current Time
CHAR* Functions::TimeStamp()
{
	SYSTEMTIME Time;
	GetLocalTime(&Time);

	sprintf_s(LocalTime, "%.2d:%.2d:%.2d", Time.wHour, Time.wMinute, Time.wSecond);

	return LocalTime;
}