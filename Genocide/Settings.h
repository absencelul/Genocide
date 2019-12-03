#define BOOLEAN		1
#define INTEGER		2
#define STRING		3
#define BLIND		4
#define STRING2		3
#define CURSE		5
struct CONFIGURATION
{
	LPCSTR Section;
	LPCSTR Name;
	DOUBLE Type;
	LPVOID Variable;
};
bool Bool(LPSTR Value);
void LoadConfig();
void Config();
void GetModuleDirectory();
int Integer(LPSTR Value);
void ParseBlind(string Value);
void SetBoolValues(LPSTR Value, bool Variable[]);
void SetKeyValues(LPSTR Value, int Variable[]);
bool CheckString(LPSTR Value);
std::string int_to_string(int t);
void SetCurseValues(LPSTR Value, bool Variable[]);
bool Curses(LPSTR Value);