#pragma once

class CCore
{
public:
	CCore( HMODULE hModule );
	~CCore( void );

	bool Initialize( void );
	void GameLoop( void );

	static CCore* s_lpcCore;

	CAutomate m_cAutomate;

private:
	HMODULE m_hModule;
	HWND m_hWnd;
	CMemory m_cMemory;

	friend class CAutomate;
};