#pragma once
class Commands
{
public:
	Commands();
	~Commands();
	static BOOL FASTCALL GameInput(WCHAR * Text);
	static int StringTokenize(char * input, char separator, char ** tokenbuf, int maxtokens);
	static void StringFix(char* szString);
	static void ReplaceString(std::string & String, std::string Find, std::string Replace);
	static bool Speak(LPSTR lpMessage, ...);
};

