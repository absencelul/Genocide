#pragma once
#define INST_INT3	0xCC
#define INST_CALL	0xE8
#define INST_NOP	0x90
#define INST_JMP	0xE9
#define INST_RET	0xC3

class Patch;

enum Dll { D2CLIENT = 0, D2COMMON, D2GFX, D2LANG, D2WIN, D2NET, D2GAME, D2LAUNCH, FOG, BNCLIENT, STORM, D2CMP, D2MULTI, D2MCPCLIENT };
enum PatchType { Call = 0, Jmp, Fill };

class Patch
{
private:
	static std::vector<Patch*>Patches;
	PatchType type;
	int offset, length, func;
	BYTE* bOldCode;
	bool injected;
public:
	Patch(PatchType type, int offset, int func, int length);

	static int GetOffset(int offset);
	void Install();
	void Uninstall();

	bool IsInstalled() { return injected; };
};
