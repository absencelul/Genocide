#define PARTY_NOT_IN_PARTY 0x00
#define PARTY_IN_PARTY 0x01
#define PARTY_INVITED_YOU 0x02
#define PARTY_INVITED_BY_YOU 0x04

#define NO_PARTY		0xFFFF

#define LOOT_BUTTON		0x01
#define MUTE_BUTTON		0x02
#define SQUELCH_BUTTON	0x03
#define HOSTILE_BUTTON	0x04

#define TOGGLE_OFF		0x00
#define TOGGLE_ON		0x01

#define INVITE_PLAYER	0x06
#define CANCEL_INVITE	0x07
#define ACCEPT_INVITE	0x08
#define LEAVE_PARTY		0x09

void Interact(DWORD UnitType, DWORD UnitId);
void TakeWP(DWORD WpID, BYTE Destination);
//void InviteAction(BYTE button, DWORD dwPlayerId);
//void ButtonAction(BYTE button, BYTE toggle, DWORD dwPlayerId);
bool SetSkill(WORD wSkillId, bool bLeft);
//BOOL SetSkill(WORD Skill, bool Left);
void UseItem(LPUNITANY Unit);
void ResynchPlayer(DWORD ID, INT Type);
void SwitchWeapon();
void Teleportz(DWORD X, DWORD Y);
void HostilePlayer(DWORD dwUnitId);
bool SetChargedSkill(WORD wSkillId, bool bLeft, DWORD dwItemId);
void UseSkillOnUnit(LPUNITANY Unit, WORD Skill, bool Left, bool Shift, bool Animation);