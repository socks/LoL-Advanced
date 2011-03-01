#pragma once

#include <map>

class CCore
{
private:
	HMODULE m_hModule;
	HWND m_hWnd;
	DWORD m_dwLastCheck;
	DWORD m_dwAttackTick;
	std::map<DWORD, float> m_cUnitHealth;

public:
	CCore( HMODULE hModule );
	~CCore( void );

	bool Initialize( void );
	void GameLoop( void );

	CMemory m_cMemory;

	static CCore* s_lpcCore;
};