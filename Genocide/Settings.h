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
class Settings
{
public:
	Settings();
	~Settings();
	static bool Bool(LPSTR Value);
	static void LoadConfig();
	static void Config();
	static void GetModuleDirectory();
	static int Integer(LPSTR Value);
	static void ParseBlind(string Value);
	static void SetBoolValues(LPSTR Value, bool Variable[]);
	static void SetKeyValues(LPSTR Value, INT Variable[]);
	static bool CheckString(LPSTR Value);
	static string to_string(INT t);
	static void SetCurseValues(LPSTR Value, bool Variable[]);
	static bool Curses(LPSTR Value);
};