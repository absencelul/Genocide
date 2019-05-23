#include "Hack.h"
vector<string> SpamMsg;
vector<string> SpamPlayers;

Helpers::Helpers()
{
}


Helpers::~Helpers()
{
}

/*void Helpers::ButtonAction(BYTE button, BYTE toggle, DWORD dwPlayerId)
{
	RosterUnit* pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext;
	BYTE Packet[7] = { 0 };
	Packet[0] = 0x5d;
	Packet[1] = button;
	Packet[2] = toggle;
	*(LPDWORD)&Packet[3] = dwPlayerId;
	D2NET_SendPacket(7, 1, Packet);
}

void Helpers::InviteAction(BYTE button, DWORD dwPlayerId)
{
	if (!AutoInvite)
		return;

	RosterUnit* pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext;
	BYTE Packet[6] = { 0 };
	Packet[0] = 0x5e;
	Packet[1] = button;
	*(LPDWORD)&Packet[2] = dwPlayerId;
	D2NET_SendPacket(6, 1, Packet);
	Funcs->InputConsole("ÿc8Auto-Invited IHPK Member");
	ButtonAction(LOOT_BUTTON, TOGGLE_ON, dwPlayerId);
}

bool Helpers::IsPlayerInAParty(RosterUnit* pPlayer)
{
	return (pPlayer->wPartyId != NO_PARTY);
}

RosterUnit* Helpers::GetRosterUnitFromId(DWORD dwPlayerId)
{
	for (RosterUnit* pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
	{
		if (pUnit->dwUnitId == dwPlayerId)
			return pUnit;
	}

	return NULL;
}*/

wchar_t* AnsiToUnicode(const char* str)
{
	wchar_t* buf = NULL;
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, buf, 0);
	buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str, -1, buf, len);
	return buf;
}

bool Spam(string Msg, string pName)
{

	for (unsigned int i = 0; i < SpamPlayers.size(); i++)
		if (!_stricmp(pName.c_str(), SpamPlayers.at(i).c_str()))
			return true;

	for (unsigned int i = 0; i < SpamMsg.size(); i++)
	{
		string SFMsg = SpamMsg.at(i);
		transform(SFMsg.begin(), SFMsg.end(), SFMsg.begin(), ::tolower);
		transform(Msg.begin(), Msg.end(), Msg.begin(), ::tolower);

		if (Msg.find(SFMsg.c_str(), 0) != std::string::npos)
		{
			Funcs->Print(0, 4, "%s was added to spam filter message (%s) filtered phrase (%s).", pName.c_str(), Msg.c_str(), SpamMsg.at(i).c_str());
			SpamPlayers.push_back(pName);
			PushConfig("Spam Players.txt", pName);
			return true;
		}
	}

	return false;
}

//Takes a file name, and pushes lines into the given vector.
void ReadConfig(string szFile, vector<string>* nVector) {
	ifstream nFile;
	string nFilePath = FilePath + szFile;
	nFile.open(nFilePath.c_str(), fstream::in);
	if (!nFile.is_open())
		return;
	char szLine[1024];
	while (nFile.getline(szLine, 1024))
		nVector->push_back(string(szLine));
	nFile.close();
}
//Takes a file name, pushes line to the end of file.
void PushConfig(string szFile, string szLine) {
	ofstream nFile;
	string nFilePath = FilePath + szFile;
	nFile.open(nFilePath.c_str(), ofstream::app);
	if (!nFile.is_open())
		return;
	nFile << szLine.c_str() << endl;
	nFile.close();
}

WCHAR * FUNCCALL wsprintfW2(WCHAR * dest, LPSTR fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	INT len = wvsprintf((LPSTR)dest, fmt, va);

	for (INT i = len; i >= 0; i--)
		dest[i] = ((LPSTR)dest)[i];

	return dest;
}

WCHAR * FUNCCALL wsprintfW2Colour(WCHAR * dest, INT col, LPSTR fmt, ...)
{
	dest[0] = 0xff;
	dest[1] = 'c';
	dest[2] = '0' + (int)col;
	dest += 3;

	va_list va;
	va_start(va, fmt);

	INT len = wvsprintf((LPSTR)dest, fmt, va);

	for (INT i = len; i >= 0; i--)
		dest[i] = ((LPSTR)dest)[i];

	return dest;
}

//Get Folder
LPSTR GetShrinesFolder(LPSTR dest, CHAR ch)
{
	GetModuleFileNameA(_dll, dest, MAX_PATH);
	PathRemoveFileSpecA(dest);
	strcat(dest, "\\Shrines\\");
	return strrchr(dest, ch) + 1;
}

LPCELLFILE InitCellFile(LPCELLFILE File)
{
	if (File) D2CMP_InitCellFile(File, &File, "?", 0, -1, "?");
	return File;
}

VOID * memcpy2(LPVOID Destination, CONST VOID * Source, size_t Count)
{
	return (LPSTR)memcpy(Destination, Source, Count) + Count;
}

HANDLE OpenFileRead(LPSTR filename)
{
	return CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

HANDLE OpenFileWrite(LPSTR filename)
{
	return CreateFile(filename, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

DWORD ReadFile(HANDLE hFile, LPVOID buf, DWORD len)
{
	DWORD numdone = 0;
	ReadFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}

LPBYTE AllocReadFile(LPSTR FileName)
{
	HANDLE hFile = OpenFileRead(FileName);
	INT FileSize = GetFileSize(hFile, 0);

	if (FileSize <= 0)
		return 0;

	LPBYTE Buffer = new BYTE[FileSize];
	ReadFile(hFile, Buffer, FileSize);

	CloseHandle(hFile);
	return Buffer;
}

//Delete Cells
VOID DeleteCellFile(LPCELLFILE File)
{
	if (File)
	{
		D2CMP_DeleteCellFile(File);
		delete File;
	}
}

LPCELLFILE LoadBMPCellFile(BYTE * Buffer, INT Width, INT Height)
{
	BYTE * Buffer2 = new BYTE[(Width * Height * 2) + Height], *Destination = Buffer2;

	for (INT i = 0; i < Height; i++)
	{
		BYTE * Source = Buffer + (i * ((Width + 3) & -4)), *Limit = Source + Width;

		while (Source < Limit)
		{
			BYTE * Start = Source, *Limit2 = min(Limit, Source + 0x7F), Transparency = !*Source;
			do Source++; while ((Transparency == (BYTE)!*Source) && (Source < Limit2));

			if (!Transparency || (Source < Limit)) *Destination++ = (BYTE)((Transparency ? 0x80 : 0) + (Source - Start));
			if (!Transparency) while (Start < Source) *Destination++ = *Start++;
		}

		*Destination++ = 0x80;
	}

	int DC6[] = { 6, 1, 0, 0xEEEEEEEE, 1, 1, 0x1C,  0, -1, -1, 0, 0, 0, -1, -1 };
	DC6[8] = Width;
	DC6[9] = Height;

	BYTE * Return = new BYTE[DC6[13] = sizeof(DC6) + (DC6[14] = (DWORD)(Destination - Buffer2)) + 3];
	memset(memcpy2(memcpy2(Return, DC6, sizeof(DC6)), Buffer2, DC6[14]), 0xEE, 3);
	delete[] Buffer2;

	return (LPCELLFILE)Return;
}

LPCELLFILE LoadBMPCellFile(LPSTR FileName)
{
	LPBYTE Return = 0;
	LPBYTE Buffer = AllocReadFile(FileName);

	BITMAPFILEHEADER * BMP1 = (BITMAPFILEHEADER *)Buffer;
	BITMAPINFOHEADER * BMP2 = (BITMAPINFOHEADER *)(Buffer + sizeof(BITMAPFILEHEADER));

	if (Buffer && (BMP1->bfType == 'MB') && (BMP2->biBitCount == 8) && (BMP2->biCompression == BI_RGB))
		Return = (LPBYTE)LoadBMPCellFile(Buffer + BMP1->bfOffBits, BMP2->biWidth, BMP2->biHeight);

	delete Buffer;
	return (LPCELLFILE)Return;
}

//Get Map Level Name
BOOL GetMapName(BYTE iMapID, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return FALSE;

	lpszBuffer[0] = '\0';
	::memset(lpszBuffer, 0, sizeof(TCHAR) * dwMaxChars);
	if (dwMaxChars == 0)
		return FALSE;

	switch (iMapID)
	{
		///////////////////////////////////////////////////
		// Act 1 Maps
		///////////////////////////////////////////////////
	case MAP_A1_ROGUE_ENCAMPMENT:
		strncpy(lpszBuffer, "Rogue Encampment", dwMaxChars);
		break;

	case MAP_A1_BLOOD_MOOR:
		strncpy(lpszBuffer, "Blood Moor", dwMaxChars);
		break;

	case MAP_A1_COLD_PLAINS:
		strncpy(lpszBuffer, "Cold Plains", dwMaxChars);
		break;

	case MAP_A1_STONY_FIELD:
		strncpy(lpszBuffer, "Stony Field", dwMaxChars);
		break;

	case MAP_A1_DARK_WOOD:
		strncpy(lpszBuffer, "Dark Wood", dwMaxChars);
		break;

	case MAP_A1_BLACK_MARSH:
		strncpy(lpszBuffer, "Black Marsh", dwMaxChars);
		break;

	case MAP_A1_DEN_OF_EVIL:
		strncpy(lpszBuffer, "Den of Evil", dwMaxChars);
		break;

	case MAP_A1_CAVE_LEVEL_1:
		strncpy(lpszBuffer, "Cave Level 1", dwMaxChars);
		break;

	case MAP_A1_UNDERGROUND_PASSAGE_LEVEL_1:
		strncpy(lpszBuffer, "Underground Passage Level 1", dwMaxChars);
		break;

	case MAP_A1_HOLE_LEVEL_1:
		strncpy(lpszBuffer, "Hole Level 1", dwMaxChars);
		break;

	case MAP_A1_PIT_LEVEL_1:
		strncpy(lpszBuffer, "Pit Level 1", dwMaxChars);
		break;

	case MAP_A1_CAVE_LEVEL_2:
		strncpy(lpszBuffer, "Cave Level 2", dwMaxChars);
		break;

	case MAP_A1_UNDERGROUND_PASSAGE_LEVEL_2:
		strncpy(lpszBuffer, "Underground Passage Level 2", dwMaxChars);
		break;

	case MAP_A1_HOLE_LEVEL_2:
		strncpy(lpszBuffer, "Hole Level 2", dwMaxChars);
		break;

	case MAP_A1_PIT_LEVEL_2:
		strncpy(lpszBuffer, "Pit Level 2", dwMaxChars);
		break;

	case MAP_A1_BURIAL_GROUNDS:
		strncpy(lpszBuffer, "Burial Grounds", dwMaxChars);
		break;

	case MAP_A1_CRYPT:
		strncpy(lpszBuffer, "Crypt", dwMaxChars);
		break;

	case MAP_A1_MAUSOLEUM:
		strncpy(lpszBuffer, "Mausoleum", dwMaxChars);
		break;

	case MAP_A1_FORGOTTEN_TOWER:
		strncpy(lpszBuffer, "Forgotten Tower", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_1:
		strncpy(lpszBuffer, "Tower Cellar Level 1", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_2:
		strncpy(lpszBuffer, "Tower Cellar Level 2", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_3:
		strncpy(lpszBuffer, "Tower Cellar Level 3", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_4:
		strncpy(lpszBuffer, "Tower Cellar Level 4", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_5:
		strncpy(lpszBuffer, "Tower Cellar Level 5", dwMaxChars);
		break;

	case MAP_A1_TAMOE_HIGHLAND:
		strncpy(lpszBuffer, "Tamoe Highland", dwMaxChars);
		break;

	case MAP_A1_MONASTERY_GATE:
		strncpy(lpszBuffer, "Monastery Gate", dwMaxChars);
		break;

	case MAP_A1_OUTER_CLOISTER:
		strncpy(lpszBuffer, "Outer Cloister", dwMaxChars);
		break;

	case MAP_A1_BARRACKS:
		strncpy(lpszBuffer, "Barracks", dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_1:
		strncpy(lpszBuffer, "Jail Level 1", dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_2:
		strncpy(lpszBuffer, "Jail Level 2", dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_3:
		strncpy(lpszBuffer, "Jail Level 3", dwMaxChars);
		break;

	case MAP_A1_INNER_CLOISTER:
	case MAP_A1_INNER_CLOISTER_2:
		strncpy(lpszBuffer, "Inner Cloister", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_1:
		strncpy(lpszBuffer, "Catacombs Level 1", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_2:
		strncpy(lpszBuffer, "Catacombs Level 2", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_3:
		strncpy(lpszBuffer, "Catacombs Level 3", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_4:
		strncpy(lpszBuffer, "Catacombs Level 4", dwMaxChars);
		break;

	case MAP_A1_TRISTRAM:
		strncpy(lpszBuffer, "Tristram", dwMaxChars);
		break;

	case MAP_A1_THE_SECRET_COW_LEVEL:
		strncpy(lpszBuffer, "The Secret Cow Level", dwMaxChars);
		break;


		///////////////////////////////////////////////////
		// Act 2 Maps
		///////////////////////////////////////////////////
	case MAP_A2_LUT_GHOLEIN:
		strncpy(lpszBuffer, "Lut Gholein", dwMaxChars);
		break;

	case MAP_A2_ROCKY_WASTE:
		strncpy(lpszBuffer, "Rocky Waste", dwMaxChars);
		break;

	case MAP_A2_DRY_HILLS:
		strncpy(lpszBuffer, "Dry Hills", dwMaxChars);
		break;

	case MAP_A2_FAR_OASIS:
		strncpy(lpszBuffer, "Far Oasis", dwMaxChars);
		break;

	case MAP_A2_LOST_CITY:
		strncpy(lpszBuffer, "Lost City", dwMaxChars);
		break;

	case MAP_A2_VALLEY_OF_SNAKES:
		strncpy(lpszBuffer, "Valley of Snakes", dwMaxChars);
		break;

	case MAP_A2_CANYON_OF_THE_MAGI:
		strncpy(lpszBuffer, "Cayon of the Magi", dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_1:
		strncpy(lpszBuffer, "Sewers Level 1", dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_2:
		strncpy(lpszBuffer, "Sewers Level 2", dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_3:
		strncpy(lpszBuffer, "Sewers Level 3", dwMaxChars);
		break;

	case MAP_A2_HAREM_LEVEL_1:
		strncpy(lpszBuffer, "Harem Level 1", dwMaxChars);
		break;

	case MAP_A2_HAREM_LEVEL_2:
		strncpy(lpszBuffer, "Harem Level 2", dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_1:
		strncpy(lpszBuffer, "Palace Cellar Level 1", dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_2:
		strncpy(lpszBuffer, "Palace Cellar Level 2", dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_3:
		strncpy(lpszBuffer, "Palace Cellar Level 3", dwMaxChars);
		break;

	case MAP_A2_STONY_TOMB_LEVEL_1:
		strncpy(lpszBuffer, "Stony Tomb Level 1", dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_1:
		strncpy(lpszBuffer, "Halls of the Dead Level 1", dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_2:
		strncpy(lpszBuffer, "Halls of the Dead Level 2", dwMaxChars);
		break;

	case MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_1:
		strncpy(lpszBuffer, "Claw Viper Temple Level 1", dwMaxChars);
		break;

	case MAP_A2_STONY_TOMB_LEVEL_2:
		strncpy(lpszBuffer, "Stony Tomb Level 2", dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_3:
		strncpy(lpszBuffer, "Halls of the Dead Level 3", dwMaxChars);
		break;

	case MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_2:
		strncpy(lpszBuffer, "Claw Viper Temple Level 2", dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_1:
		strncpy(lpszBuffer, "Maggot Lair Level 1", dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_2:
		strncpy(lpszBuffer, "Maggot Lair Level 2", dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_3:
		strncpy(lpszBuffer, "Maggot Lair Level 3", dwMaxChars);
		break;

	case MAP_A2_ANCIENT_TUNNELS:
		strncpy(lpszBuffer, "Ancient Tunnels", dwMaxChars);
		break;

	case MAP_A2_TAL_RASHAS_TOMB_1:
	case MAP_A2_TAL_RASHAS_TOMB_2:
	case MAP_A2_TAL_RASHAS_TOMB_3:
	case MAP_A2_TAL_RASHAS_TOMB_4:
	case MAP_A2_TAL_RASHAS_TOMB_5:
	case MAP_A2_TAL_RASHAS_TOMB_6:
	case MAP_A2_TAL_RASHAS_TOMB_7:
		strncpy(lpszBuffer, "Tal Rasha's Tomb", dwMaxChars);
		break;

	case MAP_A2_TAL_RASHAS_CHAMBER:
		strncpy(lpszBuffer, "Tal Rasha's Chamber", dwMaxChars);
		break;

	case MAP_A2_ARCANE_SANCTUARY:
		strncpy(lpszBuffer, "Arcane Sanctuary", dwMaxChars);
		break;


		///////////////////////////////////////////////////
		// Act 3 Maps
		///////////////////////////////////////////////////
	case MAP_A3_KURAST_DOCKS:
		strncpy(lpszBuffer, "Kurast Docks", dwMaxChars);
		break;

	case MAP_A3_SPIDER_FOREST:
		strncpy(lpszBuffer, "Spider Forest", dwMaxChars);
		break;

	case MAP_A3_GREAT_MARSH:
		strncpy(lpszBuffer, "Great Marsh", dwMaxChars);
		break;

	case MAP_A3_FLAYER_JUNGLE:
		strncpy(lpszBuffer, "Flayer Jungle", dwMaxChars);
		break;

	case MAP_A3_LOWER_KURAST:
		strncpy(lpszBuffer, "Lower Kurast", dwMaxChars);
		break;

	case MAP_A3_KURAST_BAZAAR:
		strncpy(lpszBuffer, "Kurast Bazaar", dwMaxChars);
		break;

	case MAP_A3_UPPER_KURAST:
		strncpy(lpszBuffer, "Upper Kurast", dwMaxChars);
		break;

	case MAP_A3_KURAST_CAUSEWAY:
		strncpy(lpszBuffer, "Kurast Causeway", dwMaxChars);
		break;

	case MAP_A3_TRAVINCAL:
		strncpy(lpszBuffer, "Travincal", dwMaxChars);
		break;

	case MAP_A3_ARCHNID_LAIR:
		strncpy(lpszBuffer, "Arachnid Lair", dwMaxChars);
		break;

	case MAP_A3_SPIDER_CAVERN:
		strncpy(lpszBuffer, "Spider Cavern", dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_1:
		strncpy(lpszBuffer, "Swampy Pit Level 1", dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_2:
		strncpy(lpszBuffer, "Swampy Pit Level 2", dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_1:
		strncpy(lpszBuffer, "Flayer Dungeon Level 1", dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_2:
		strncpy(lpszBuffer, "Flayer Dungeon Level 2", dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_3:
		strncpy(lpszBuffer, "Swampy Pit Level 3", dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_3:
		strncpy(lpszBuffer, "Flayer Dungeon Level 3", dwMaxChars);
		break;

	case MAP_A3_SEWERS_LEVEL_1:
		strncpy(lpszBuffer, "Sewers Level 1", dwMaxChars);
		break;

	case MAP_A3_SEWERS_LEVEL_2:
		strncpy(lpszBuffer, "Sewers Level 2", dwMaxChars);
		break;

	case MAP_A3_RUINED_TEMPLE:
		strncpy(lpszBuffer, "Ruined Temple", dwMaxChars);
		break;

	case MAP_A3_DISUSED_FANE:
		strncpy(lpszBuffer, "Disused Fane", dwMaxChars);
		break;

	case MAP_A3_FORGOTTEN_RELIQUARY:
		strncpy(lpszBuffer, "Forgotten Reliquary", dwMaxChars);
		break;

	case MAP_A3_FORGOTTEN_TEMPLE:
		strncpy(lpszBuffer, "Forgotten Temple", dwMaxChars);
		break;

	case MAP_A3_RUINED_FANE:
		strncpy(lpszBuffer, "Ruined Fane", dwMaxChars);
		break;

	case MAP_A3_DISUSED_RELIQUARY:
		strncpy(lpszBuffer, "Disused Reliquary", dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_1:
		strncpy(lpszBuffer, "Durance of Hate Level 1", dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_2:
		strncpy(lpszBuffer, "Durance of Hate Level 2", dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_3:
		strncpy(lpszBuffer, "Durance of Hate Level 3", dwMaxChars);
		break;

		///////////////////////////////////////////////////
		// Act 4 Maps
		///////////////////////////////////////////////////
	case MAP_A4_THE_PANDEMONIUM_FORTRESS:
		strncpy(lpszBuffer, "The Pandemonium Fortress", dwMaxChars);
		break;

	case MAP_A4_OUTER_STEPPES:
		strncpy(lpszBuffer, "Outer Steppes", dwMaxChars);
		break;

	case MAP_A4_PLAINS_OF_DESPAIR:
		strncpy(lpszBuffer, "Plains of Despair", dwMaxChars);
		break;

	case MAP_A4_CITY_OF_THE_DAMNED:
		strncpy(lpszBuffer, "City of the Damned", dwMaxChars);
		break;

	case MAP_A4_RIVER_OF_FLAME:
		strncpy(lpszBuffer, "River of Flame", dwMaxChars);
		break;

	case MAP_A4_THE_CHAOS_SANCTUARY:
		strncpy(lpszBuffer, "The Chaos Sanctuary", dwMaxChars);
		break;

		///////////////////////////////////////////////////
		// Act 5 Maps
		///////////////////////////////////////////////////
	case MAP_A5_HARROGATH:
		strncpy(lpszBuffer, "Harrogath", dwMaxChars);
		break;

	case MAP_A5_THE_BLOODY_FOOTHILLS:
		strncpy(lpszBuffer, "The Bloody Foothills", dwMaxChars);
		break;

	case MAP_A5_FRIGID_HIGHLANDS:
		strncpy(lpszBuffer, "Frigid Highlands", dwMaxChars);
		break;

	case MAP_A5_ARREAT_PLATEAU:
		strncpy(lpszBuffer, "Arreat Plateau", dwMaxChars);
		break;

	case MAP_A5_CRYSTALLINE_PASSAGE:
		strncpy(lpszBuffer, "Crystalline Passage", dwMaxChars);
		break;

	case MAP_A5_FROZEN_RIVER:
		strncpy(lpszBuffer, "Frozen River", dwMaxChars);
		break;

	case MAP_A5_GLACIAL_TRAIL:
		strncpy(lpszBuffer, "Glacial Trail", dwMaxChars);
		break;

	case MAP_A5_DRIFTER_CAVERN:
		strncpy(lpszBuffer, "Drifter Cavern", dwMaxChars);
		break;

	case MAP_A5_FROZEN_TUNDRA:
		strncpy(lpszBuffer, "Frozen Tundra", dwMaxChars);
		break;

	case MAP_A5_THE_ANCIENTS_WAY:
		strncpy(lpszBuffer, "The Ancients' Way", dwMaxChars);
		break;

	case MAP_A5_ICY_CELLAR:
		strncpy(lpszBuffer, "Icy Cellar", dwMaxChars);
		break;

	case MAP_A5_ARREAT_SUMMIT:
		strncpy(lpszBuffer, "Arreat Summit", dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_PAIN:
		strncpy(lpszBuffer, "Halls of Pain", dwMaxChars);
		break;

	case MAP_A5_ABADDON:
		strncpy(lpszBuffer, "Abaddon", dwMaxChars);
		break;

	case MAP_A5_PIT_OF_ACHERON:
		strncpy(lpszBuffer, "Pit of Acheron", dwMaxChars);
		break;

	case MAP_A5_INFERNAL_PIT:
		strncpy(lpszBuffer, "Infernal Pit", dwMaxChars);
		break;

	case MAP_A5_NIHLATHAKS_TEMPLE:
		strncpy(lpszBuffer, "Nihlathak's Temple", dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_ANGUISH:
		strncpy(lpszBuffer, "Halls of Anguish", dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_VAUGHT:
		strncpy(lpszBuffer, "Halls of Vaught", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_1:
		strncpy(lpszBuffer, "Worldstone Keep Level 1", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_2:
		strncpy(lpszBuffer, "Worldstone Keep Level 2", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_3:
		strncpy(lpszBuffer, "Worldstone Keep Level 3", dwMaxChars);
		break;

	case MAP_A5_THRONE_OF_DESTRUCTION:
		strncpy(lpszBuffer, "Throne of Destruction", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP:
		strncpy(lpszBuffer, "The Worldstone Chamber", dwMaxChars);
		break;

	default:
		return 0;
		break;
	}

	return FALSE;
}