#pragma once

class CCore
{
private:
	HMODULE m_hModule;
	HWND m_hWnd;
	DWORD m_dwLastCheck;

public:
	CCore( HMODULE hModule );
	~CCore( void );

	bool Initialize( void );
//	bool BeforePacketReceived( BYTE yType, BYTE* lpyPacket, DWORD dwLength );
	void GameLoop( void );

	CMemory m_cMemory;

	static CCore* s_lpcCore;
};