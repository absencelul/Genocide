extern vector<string> SpamMsg;
extern vector<string> SpamPlayers;

class Helpers
{
public:
	Helpers();
	~Helpers();
	//static void ButtonAction(BYTE button, BYTE toggle, DWORD dwPlayerId);
	//static void InviteAction(BYTE button, DWORD dwPlayerId);
	//static bool IsPlayerInAParty(RosterUnit* pPlayer);
	//static RosterUnit* GetRosterUnitFromId(DWORD dwPlayerId);
};

wchar_t* AnsiToUnicode(const char* str);
char* UnicodeToAnsi(const wchar_t* str);
bool Spam(string Msg, string pName);
void ReadConfig(string szFile, vector<string>* nVector);
void PushConfig(string szFile, string szLine);
WCHAR * FUNCCALL wsprintfW2(WCHAR * dest, LPSTR fmt, ...);
WCHAR * FUNCCALL wsprintfW2Colour(WCHAR * dest, INT col, LPSTR fmt, ...);
LPSTR GetShrinesFolder(LPSTR dest, CHAR ch);
LPCELLFILE InitCellFile(LPCELLFILE File);
VOID * memcpy2(LPVOID Destination, CONST VOID * Source, size_t Count);
HANDLE OpenFileRead(LPSTR filename);
HANDLE OpenFileWrite(LPSTR filename);
DWORD ReadFile(HANDLE hFile, LPVOID buf, DWORD len);
LPCELLFILE LoadBMPCellFile(BYTE * Buffer, INT Width, INT Height);
LPCELLFILE LoadBMPCellFile(LPSTR FileName);
LPBYTE AllocReadFile(LPSTR FileName);
VOID DeleteCellFile(LPCELLFILE File);
BOOL GetMapName(BYTE iMapID, LPSTR lpszBuffer, DWORD dwMaxChars);