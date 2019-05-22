#define ATTACKTYPE_LEFT				0xC5
#define ATTACKTYPE_SHIFTLEFT		0xE5
#define ATTACKTYPE_RIGHT			0x46
#define ATTACKTYPE_SHIFTRIGHT		0x66

typedef struct TargetInfo_t
{
	CHAR PlayerName[0x10];
	DWORD UnitId;
	DWORD ClassId;
	BYTE Level;
	BYTE Life;
	DWORD X;
	DWORD Y;
	DWORD HammerX;
	DWORD HammerY;
	DWORD Resists[6];
} TARGETINFO, *PTARGETINFO;

void EnumeratePlayers();
void AttackNearest(bool Left);
void SetBlind(WORD x, WORD y, WORD xHammer, WORD yHammer, DWORD UnitId);
void NextTar();
void PrevTar();
void Attack(bool Left);
DWORD __fastcall GameAttack(AttackStruct* pAttack);