typedef struct PacketedAttack_t
{
	WORD SkillID;
	bool Shift;
} PACKETATTACK, * PPACKETATTACK;

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
} TARGETINFO, * PTARGETINFO;

void EnumeratePlayers();
void Attack(DWORD x, DWORD y, bool Left, bool Shift, bool attackUnit, LPUNITANY Unit);
void AttackTarget(bool Left);
void AttackNearest(bool Left);
void TestKey();
void SetBlind(WORD x, WORD y, WORD xHammer, WORD yHammer, DWORD UnitId);
void PrevTar();
void NextTar();