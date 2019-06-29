#include "Hack.h"

Stubs::Stubs()
{
}


Stubs::~Stubs()
{
}

void __declspec(naked) Stubs::OnGameLoopSTUB()
{
	__asm
	{
		pushad;
		call OnGameLoop;
		popad;

		push D2CLIENT_GameLoop;
		ret;
	}
}

DWORD __stdcall Stubs::ChickenSTUB(UnitAny* pUnit, DWORD dwStat, DWORD dwStat2)
{
	//Print(0, 4, "%d ms, Life: %d%% - Mana: %d%%", (GetTickCount() - TestDelay), LifeMana(true), LifeMana(false));
	//TestDelay = GetTickCount();
	Rescue();
	return D2COMMON_GetUnitStat(pUnit, dwStat, dwStat2);
}

void __declspec(naked) __fastcall Stubs::D2CLIENT_Attack(AttackStruct* Attack, BOOL AttackingUnit)
{
	__asm
	{
		push esi;
		mov esi, ecx;
		push edx;
		call D2CLIENT_Attack_I;
		pop esi;
		ret;
	}
}

void __declspec(naked) Stubs::DrawAutomapSTUB()
{
	__asm
	{
		PUSHAD;
		CALL DrawAutomap;
		POPAD;
		POP EDI;
		POP ESI;
		RET;
	}
}

void __declspec(naked) Stubs::DrawInterfaceSTUB()
{
	__asm
	{
		call DrawScreen;
		POP EBX;
		MOV ESP, EBP;
		POP EBP;
		ret
	}
}

DWORD __declspec(naked) _fastcall Stubs::DrawRectStub(RECT* pRect) {
	__asm
	{
		mov eax, ecx
		jmp D2CLIENT_DrawRectFrame
	}
}

void __declspec(naked) Stubs::GameFailToJoinSTUB()
{
	__asm
	{
		cmp esi, 4000;
		ret;
	}
}

void __declspec(naked) Stubs::GamePacketReceived_STUB()
{
	/*__asm
	{
		PUSH ECX;
		PUSH EDX;
		call OnGamePacketReceived;
		test eax, eax;
		POP EDX;
		POP ECX;
		jnz OldCode;
		mov edx, 0;
	OldCode:
		JMP D2NET_ReceivePacket_I;
	}*/

	__asm
	{
		pushad;

		call OnGamePacketReceived;
		test eax, eax;

		popad;
		jnz OldCode;

		mov edx, 0;

	OldCode:
		mov eax, [esp + 4];
		push eax;
		call D2NET_ReceivePacket_I;

		ret 4;
	}
}

DWORD NAKED FASTCALL Stubs::GetUIVarSTUB(DWORD UI)
{
	__asm
	{
		MOV EAX, ECX;
		JMP D2CLIENT_GetUiVar_I;
	}
}

DWORD __declspec(naked) __fastcall Stubs::GetUnitSTUB(DWORD dwUnitId, DWORD dwUnitType)
{
	__asm
	{
		POP EAX;
		PUSH EDX;
		PUSH EAX;
		SHL EDX, 9;
		MOV EAX, D2CLIENT_GetUnitFromId_I;
		ADD EDX, EAX;
		MOV EAX, ECX;
		AND EAX, 0x7F;
		JMP D2CLIENT_GetUnitFromId_II;
	}
}

DWORD NAKED FASTCALL Stubs::GetUnitNameSTUB(DWORD Unit)
{
	__asm
	{
		MOV EAX, ECX;
		JMP D2CLIENT_GetUnitName_I;
	}
}

void __declspec(naked) Stubs::KeyHookSTUB()
{
	__asm
	{
		CALL D2CLIENT_DrawEnteringLevelText;
		CALL KeyHooks::KeyHook;
		RETN;
	}
}

void __declspec(naked) Stubs::OnGameEndSTUB()
{
	__asm
	{
		CALL OnGameEnd;
		MOV ECX, 0xB4;
		RET;
	}
}

void __declspec(naked) Stubs::OnGameKeyDownSTUB()
{
	__asm
	{
		MOV CL, [EDI + 0x08];
		MOV DL, [EDI + 0x0C + 3];
		AND DL, 0x40;
		CALL KeyHooks::OnGameKeyDown;
		TEST BYTE PTR[EDI + 0x0C + 3], 0x40;
		RET;
	}
}

void __declspec(naked) Stubs::RefreshGameListSTUB()
{
	__asm
	{
		CMP EAX, 0x1388;
		RETN;
	}
}

void __declspec(naked) Stubs::ViewInventorySTUB1()
{
	__asm
	{
		//push eax;
		call ViewInventoryPatch1;
		mov esi, eax;
		//pop eax;
		ret;
	}
}

void __declspec(naked) Stubs::ViewInventorySTUB2()
{
	__asm
	{
		//push eax;
		call ViewInventoryPatch1;
		mov ebx, eax;
		//pop eax;
		ret;
	}
}

void __declspec(naked) Stubs::ViewInventorySTUB3()
{
	__asm
	{
		push eax
		call ViewInventoryPatch1
		mov edi, eax
		pop eax
		mov esi, [edi + 60h]
		lea ecx, [eax + 17h]
		ret
	}
}

void __declspec(naked) Stubs::InputSTUB()
{
	__asm
	{
		PUSHAD;
		MOV ECX, EBX;
		CALL Commands::GameInput;
		CMP EAX, 0;
		POPAD;
		JE Block;
		CALL D2CLIENT_InputCall_I;
		RET;
	Block:
		XOR EAX, EAX;
		RET;
	}
}

/*void __declspec(naked) GameAttack_Intercept()
{
	__asm //1.14d
	{
	pushad
	mov ecx, esi // attack struct
	call GameAttack
	popad

	cmp eax, -1
	je OldCode

	call D2CLIENT_GetSelectedUnit

	cmp eax, 0
	je OldCode

	mov[esp + 0x8 + 0x4 + 0x4], 1 // bool unit

	OldCode:
	mov eax, [p_D2CLIENT_MouseY]
	mov eax, [eax]
	retn
	}
}*/

bool FASTCALL Stubs::ChatText(const wchar_t *Text, int xPos, int yPos, DWORD Color, DWORD dwUnk)
{
	Color = colorText;
	return D2WIN_DrawText(Text, xPos, yPos, Color, dwUnk);
}

DWORD NAKED FASTCALL Stubs::D2CLIENT_InitAutomapLayer_STUB(DWORD nLayerNo)
{
	__asm
	{
		push edi
		mov edi, ecx;
		call D2CLIENT_InitAutomapLayer_I;
		pop edi;
		ret;
	}
}