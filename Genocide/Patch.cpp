#include "Hack.h"
std::vector<Patch*> Patch::Patches;

Patch::Patch(PatchType type, int offset, int func, int length)
	: type(type), offset(offset), func(func), length(length) {
	bOldCode = new BYTE[length];
	injected = false;
	Patches.push_back(this);
}

int Patch::GetOffset(int offset)
{
	//Get the module of Game.exe
	HMODULE Dll = GetModuleHandle("Game.exe");

	//If Dll failed to load, return false
	if (!Dll)
		return false;

	//Check if it is an ordinal, if so, get proper address
	if (offset < 0)
		return (DWORD)GetProcAddress(Dll, (LPCSTR)(-offset));

	//If just regular offset, add the two
	return ((DWORD)Dll) + offset;
}

void Patch::Install()
{
	//Check if we've installed this patch
	if (IsInstalled())
		return;

	//Initialize variables
	BYTE* code = new BYTE[length];
	DWORD protect;

	//Get the address that we are Patch
	int address = GetOffset(offset);

	//Read the old code to allow patch removal
	::ReadProcessMemory(GetCurrentProcess(), (void*)address, bOldCode, length, NULL);

	::memset(code, (type == Fill) ? (BYTE)func : 0x90, length);

	if (type != Fill)
	{
		//Set CALL / JMP opcode
		code[0] = (type == Call) ? INST_CALL : INST_JMP;

		//Set address to redirect to
		*(DWORD*)& code[1] = func - (address + 5);
	}

	//Write patch in
	VirtualProtect((VOID*)address, length, PAGE_EXECUTE_READWRITE, &protect);
	memcpy_s((VOID*)address, length, code, length);
	VirtualProtect((VOID*)address, length, protect, &protect);

	delete[] code;

	//Set var that we successfully patched
	injected = true;
}

void Patch::Uninstall()
{
	//Check if patch is installed
	if (!IsInstalled())
		return;

	//Get the address
	int address = GetOffset(offset);
	DWORD protect;

	//Revert to previous patch
	VirtualProtect((VOID*)address, length, PAGE_EXECUTE_READWRITE, &protect);
	memcpy_s((VOID*)address, length, bOldCode, length);
	VirtualProtect((VOID*)address, length, protect, &protect);

	injected = false;
}