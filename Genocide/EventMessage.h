#pragma once
class EventMessage
{
public:
	EventMessage();
	~EventMessage();
	DWORD EventMessageHandler(LPBYTE Packet, DWORD Length);
};

