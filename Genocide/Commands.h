#pragma once

bool __fastcall GameInput(WCHAR* Text);
int StringTokenize(char* input, char separator, char** tokenbuf, int maxtokens);
//static void StringFix(char* szString);
//static void ReplaceString(std::string & String, std::string Find, std::string Replace);
bool Speak(LPSTR lpMessage, ...);
