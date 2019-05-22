struct AUTOMAP
{
	LPSTR Message;
	string Data;
	int FontColor;
	int FontType;
};

void DrawAutomap();
void GameInfo();
void xVector();
void PlayerInfo();
void DrawCross(INT X, INT Y, DWORD Color, BOOL Automap);
void FCPointer();
void AddStringToList(std::list<std::string> & StringList, LPSTR lpFormat, ...);
char* LevelNameInfo();