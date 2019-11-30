extern HANDLE portalThread;
extern DWORD ATPID;

bool Portal(LPBYTE Packet, DWORD Length);
//BOOL __fastcall OnMessage(BYTE* Packet, DWORD Length);
DWORD WINAPI EnterPortal(LPVOID dwTimer);
DWORD FASTCALL OnGamePacketReceived(LPBYTE Packet, DWORD Length);