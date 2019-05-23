#include "Hack.h"

Commands::Commands()
{
}


Commands::~Commands()
{
}

bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}


BOOL FASTCALL Commands::GameInput(WCHAR * Text)
{
	char szBuffer[255];
	char* Argument[32];

	WideCharToMultiByte(CP_ACP, 0, Text, -1, szBuffer, sizeof(szBuffer), 0, 0);
	StringTokenize(szBuffer + 1, ' ', Argument, 32);

	if (HideCommands && szBuffer[0] != ';' && _stricmp(Argument[0], "hc"))
		return true;

	switch (szBuffer[0])
	{
	case ';':
		// Hide Commands
		if (!_stricmp(Argument[0], "hc"))
		{
			HideCommands = !HideCommands;
			Funcs->InputConsole(HideCommands ? "Commands are Off!" : "Commands are On!");
			return false;
		}
		break;
	case '.':
		// Reload Configuration
		if (!_stricmp(Argument[0], "reload"))
		{
			Settings::LoadConfig();
			Funcs->InputConsole("Config.ini was reloaded.");
			return false;
		}

		// Killing Score
		if (!_stricmp(Argument[0], "score"))
		{
			Speak("Your total kill count since injection: %d", killCount);
			Speak("Your total kill count this game: %d", killCountGame);
			return false;
		}

		// Next target
		if (!_stricmp(Argument[0], "next"))
		{
			AA->NextTar();
			Input = GetTickCount64();
			return false;
		}

		// Previous target
		if (!_stricmp(Argument[0], "prev"))
		{
			AA->PrevTar();
			Input = GetTickCount64();
			return false;
		}

		// Hammer Angle
		if (!_stricmp(Argument[0], "ha"))
		{
			if (hammer[2] < 2)
				hammer[2]++;
			else hammer[2] = 0;

			switch (hammer[2])
			{
			case 0:
				Funcs->InputConsole("Hammer Angle is Disabled.");
				break;

			case 1:
				Funcs->InputConsole("Hammer Angle is Enabled.");
				break;

			case 2:
				Funcs->InputConsole("Hammer Angle is angling at Mouse.");
				break;
			}
			return false;
		}

		// Print Ping
		if (!_stricmp(Argument[0], "p"))
		{
			Speak("FPS: %d, Skip: %d, Ping: %d", *p_D2CLIENT_FPS, *p_D2CLIENT_Skip, *p_D2CLIENT_Ping);
			return false;
		}

		if (!_stricmp(Argument[0], "as"))
		{
			Auras = !Auras;
			Funcs->InputConsole(Auras ? "Aura Swap is Enabled." : "Aura Swap is Disabled.");
			return false;
		}

		if (!_stricmp(Argument[0], "tl"))
		{
			if (Argument[1] && is_number(Argument[1]))
			{
				Chickens[0] = atoi(Argument[1]);

				if (Chickens[0] >= 0)
					Funcs->InputConsole("Escaping to town at %s", Argument[1]);
			}
			else if (Argument[1] && !_stricmp(Argument[1], "off"))
			{
				Chickens[0] = -1;
				Funcs->InputConsole("Escaping to town is Disabled");
			}
			return false;
		}

		if (!_stricmp(Argument[0], "tm"))
		{
			if (Argument[1] && is_number(Argument[1]))
			{
				Chickens[2] = atoi(Argument[1]);

				if (Chickens[0] >= 0)
					Funcs->InputConsole("Escaping to town at %s", Argument[1]);
			}
			else if (Argument[1] && !_stricmp(Argument[1], "off"))
			{
				Chickens[2] = -1;
				Funcs->InputConsole("Escaping to town is Disabled");
			}
			return false;
		}

		if (!_stricmp(Argument[0], "el"))
		{
			if (Argument[1] && is_number(Argument[1]))
			{
				Chickens[1] = atoi(Argument[1]);

				if (Chickens[0] >= 0)
					Funcs->InputConsole("Escaping out of game at %s", Argument[1]);
			}
			else if (Argument[1] && !_stricmp(Argument[1], "off"))
			{
				Chickens[1] = -1;
				Funcs->InputConsole("Escaping out of game is Disabled");
			}
			return false;
		}

		if (!_stricmp(Argument[0], "em"))
		{
			if (Argument[1] && is_number(Argument[1]))
			{
				Chickens[3] = atoi(Argument[1]);

				if (Chickens[0] >= 0)
					Funcs->InputConsole("Escaping out of game at %s", Argument[1]);
			}
			else if (Argument[1] && !_stricmp(Argument[1], "off"))
			{
				Chickens[3] = -1;
				Funcs->InputConsole("Escaping out of game is Disabled");
			}
			return false;
		}

		break;
	}

	/*if (HideCommands)
		return true;*/

	if (szBuffer[0] == '.')
	{
		///*Reload*/
		//if (!_stricmp(Argument[0], "reload"))
		//{
		//	Settings::LoadConfig();
		//	Funcs->InputConsole("Configuration was reloaded!");

		//	return NULL;
		//}

		//// Kill Score
		//if (!_stricmp(Argument[0], "score"))
		//{
		//	Speak("Current kill count: %d", killCount);
		//	Speak("Kill count this game: %d", killCountGame);
		//	return NULL;
		//}

		///*Block Portal*/
		//if (!_stricmp(Argument[0], "btp"))
		//{
		//	PortalStates[1] = !PortalStates[1];
		//	Funcs->InputConsole(0, 4, PortalStates ? "Privilege�c0: Blocking town portal interaction when towned by life/mana %%" : "Privilege�c0: �c1Not blocking town portal interaction when towned by life/mana %%");
		//	return false;
		//}

		///*Next Target*/
		//if (!_stricmp(Argument[0], "next"))
		//{
		//	AA->NextTar();
		//	Input = GetTickCount64();
		//	return NULL;
		//}

		///*Last Target*/
		//if (!_stricmp(Argument[0], "last"))
		//{
		//	AA->PrevTar();
		//	Input = GetTickCount64();
		//	return NULL;
		//}

		////Smart Angle
		//if (!_stricmp(Argument[0], "sa"))
		//{
		//	if (hammer[2] < 2)
		//		hammer[2]++;
		//	else hammer[2] = 0;

		//	switch (hammer[2])
		//	{
		//	case 0:
		//		Funcs->InputConsole("Smart Angle is Disabled.");
		//		break;

		//	case 1:
		//		Funcs->InputConsole("Smart Angle is Enabled.");
		//		break;

		//	case 2:
		//		Funcs->InputConsole("Smart Angle is angling at Mouse.");
		//		break;
		//	}
		//	return NULL;
		//}

		///*PING CHECK*/
		//if (!_stricmp(Argument[0], "p"))
		//{
		//	Speak("FPS: %d Skip: %d Ping: %d", FPS, Skip, Ping);
		//	return NULL;
		//}

		////Aura Flash
		//if (!_stricmp(Argument[0], "as"))
		//{
		//	Auras = !Auras;
		//	Funcs->InputConsole(Auras ? "Aura Shifting is now [Enabled]." : "Aura Shifting is now [Disabled].");
		//	return NULL;
		//}

		///*Town Life*/
		//if (!_stricmp(Argument[0], "tl"))
		//{
		//	if (Argument[1])
		//	{
		//		Chickens[0] = atoi(Argument[1]);

		//		if (Chickens[0] >= 0)
		//			Funcs->InputConsole("Escape�c0: �c4Escaping to town at �c1%s percent.", Argument[1]);
		//		else
		//			Funcs->InputConsole("Escape�c0: Escaping to town at �c1life �c0is off");

		//		return false;
		//	}
		//}

		///*Town Mana*/
		//if (!_stricmp(Argument[0], "tm"))
		//{
		//	if (Argument[1])
		//	{
		//		Chickens[2] = atoi(Argument[1]);

		//		if (Chickens[2] >= 0)
		//			Funcs->InputConsole("Escape�c0: �c4Escaping to town at �c3%s mana", Argument[1]);
		//		else
		//			Funcs->InputConsole("Escape�c0: �c0Escaping to town at �c3mana �c0is off");

		//		return false;
		//	}
		//}

		///*Exit Life*/
		//if (!_stricmp(Argument[0], "el"))
		//{
		//	if (Argument[1])
		//	{
		//		Chickens[1] = atoi(Argument[1]);

		//		if (Chickens[1] >= 0)
		//			Funcs->InputConsole("Rescue�c0: �c4Escaping out of game at �c1%s percent", Argument[1]);
		//		else
		//			Funcs->InputConsole("Rescue�c0: Escaping out of game at �c1life �c0is off");

		//		return false;
		//	}
		//}

		///*Exit Mana*/
		//if (!_stricmp(Argument[0], "em"))
		//{
		//	if (Argument[1])
		//	{
		//		Chickens[3] = atoi(Argument[1]);

		//		if (Chickens[3] >= 0)
		//			Funcs->InputConsole(0, 4, "Rescue�c0: �c4Escaping out of game at �c3%s%% mana", Argument[1]);
		//		else
		//			Funcs->InputConsole(0, 4, "Rescue�c0: Escaping out of game at �c3mana �c0is off");

		//		return false;
		//	}
		//}

		if (!_stricmp(Argument[0], "hp"))
		{
			if (Argument[1])
			{
				Potions[0] = atoi(Argument[1]);

				if (Potions[0] >= 0)
					Funcs->InputConsole("AutoPot�c0: �c4Drinking health potions at �c1%s life", Argument[1]);
				else
					Funcs->InputConsole("AutoPot�c0: Drinking health potions at �c1life �c0is off");

				return false;
			}
		}

		if (!_stricmp(Argument[0], "mp"))
		{
			if (Argument[1])
			{
				Potions[2] = atoi(Argument[1]);

				if (Potions[2] >= 0)
					Funcs->InputConsole("AutoPot�c0: �c4Drinking mana potions at �c3%s mana", Argument[1]);
				else
					Funcs->InputConsole("AutoPot�c0: Drinking mana potions at �c3mana %% �c0is off");

				return false;
			}
		}

		if (!_stricmp(Argument[0], "rl"))
		{
			if (Argument[1])
			{
				Potions[1] = atoi(Argument[1]);

				if (Potions[1] >= 0)
					Funcs->InputConsole("Potion�c0: �c4Drinking rejuvenation potions at �c1%s life", Argument[1]);
				else
					Funcs->InputConsole("Potion�c0: Drinking rejuvenation potions at �c1life %% �c0is off");

				return false;
			}
		}

		if (!_stricmp(Argument[0], "rm"))
		{
			if (Argument[1])
			{
				Potions[3] = atoi(Argument[1]);

				if (Potions[3] >= 0)
					Funcs->InputConsole("Potion�c0: �c4Drinking rejuvenation potions at �c3%s mana", Argument[1]);
				else
					Funcs->InputConsole("Potion�c0: Drinking rejuvenation potions at �c3mana �c0is off");

				return false;
			}
		}

	}
	return true;
}

int Commands::StringTokenize(char * input, char separator, char ** tokenbuf, int maxtokens)
{
	char * p = input;
	int i = 0;
	do {
		tokenbuf[i] = p;
		p = strchr(p, separator);
		if (p)
			*(p++) = 0;
	} while (p && ++i < maxtokens);
	return ++i;
}

void Commands::StringFix(char* szString)
{
	char szBuffer[1024] = { 0 };

	for (unsigned int i = 0; i < strlen(szString); ++i)
	{
		if (szString[i] == '	' || szString[i] == '/n' || szString[i] == '/r')
			continue;

		szBuffer[i] = szString[i];
	}

	strcpy(szString, szBuffer);
}

void Commands::ReplaceString(std::string & String, std::string Find, std::string Replace)
{
	for (auto i = String.find(Find); i != std::string::npos; i = String.find(Find))
	{
		String.erase(i, Find.size());
		String.insert(i, Replace);
	}
}

bool Commands::Speak(LPSTR lpMessage, ...)
{
	if (!Me)
		return false;

	if (strlen(lpMessage) > 0)
	{
		char szBuffer[0x400] = "";
		va_list Args;

		va_start(Args, lpMessage);
		vsprintf(szBuffer, lpMessage, Args);
		va_end(Args);

		LPBYTE Packet = new BYTE[(int)strlen(szBuffer) + 6];
		memset(Packet, 0, (int)strlen(szBuffer) + 6);

		Packet[0] = 0x15;
		*(LPWORD)&Packet[1] = 1;
		memcpy(Packet + 3, szBuffer, (int)strlen(szBuffer));

		D2NET_SendPacket((int)strlen(szBuffer) + 6, 1, Packet);
		delete[] Packet;
	}

	return true;
}