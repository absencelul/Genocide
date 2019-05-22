#pragma once
class Enchant
{
public:
	Enchant();
	~Enchant();

	static DWORD WINAPI ChantThread(LPVOID Parameter);
	static void enChant();
};

