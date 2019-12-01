typedef struct ConsoleMessage_t {
	std::string ConsoleString;
	ULONGLONG Timer;
} CONSOLEMESSAGE, * LPCONSOLEMESSAGE;

enum BoxTrans { BTThreeFourths, BTOneHalf, BTOneFourth, BTWhite, BTBlack, BTNormal, BTScreen, BTHighlight, BTFull };
enum TextColor { Disabled = -1, White = 0, Red, Green, Blue, Gold, Grey, Black, Tan, Orange, Yellow, DarkGreen, Purple, Silver = 15 };
enum Alignment { None = 0, Center = 1, Right = 2, Top = 4 };

void DrawScreen();
DWORD WINAPI EnchThread(LPVOID Param);
void extraDraw();
void DrawBoxA(int x1, int y1, int x2, int y2, int nLineColor, int nBackGroundColor, int nTrans);
void OnDrawBox(int x, int xSize, int y, int ySize, DWORD dwColor, BoxTrans trans);
POINT GetTextSize(string text, unsigned int font);
void OnDrawTextBox(int x, int y, DWORD dwBGColor, BoxTrans trans, TextColor color, unsigned int font, std::string text, ...);
void BoxHook(unsigned int x, unsigned int y, unsigned int xSize, unsigned int ySize, DWORD dwColor, BoxTrans Trans);
void DrawOutOrbs();
void TextHook(int x, int y, TextColor color, Alignment align, unsigned int font, bool automap, std::string text, ...);
//void CharToWide(char* lpMultiByteStr, int cbMultiByte, wchar_t* lpWideCharStr, int cchWideChar);
//void MyMultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, INT cbMultiByte, LPWSTR lpWideCharStr, INT cchWideChar);
//void DrawTextX(int X, int Y, int Color, int Cent, int TransLvl, char* Msg, ...);
void AuraSwap();
//INT TextWidth(LPSTR szText);
void DrawOutConsole();
void DrawPlayerBlob(int xpos, int ypos, int col);
VOID MapToScreenCoords(POINT& rMapPosition);
VOID ScreenToAutomapRelative(POINT* pos, INT x, INT y);
int GetOrbColor(DWORD nMax);
void TimerBo();
void TargetList();