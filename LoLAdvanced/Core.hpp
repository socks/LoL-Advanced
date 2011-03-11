#pragma once

#include <string>

class CCore
{
public:
	CCore( HMODULE hModule );
	~CCore( void );

	bool Initialize( void );
	void GameLoop( void );
	void Print( std::string szOutput );

	static CCore* s_lpcCore;

	CAutomate m_cAutomate;

private:
	HMODULE m_hModule;
	HWND m_hWnd;
	CMemory m_cMemory;

	bool m_bFirstLoop;

	friend class CAutomate;
};