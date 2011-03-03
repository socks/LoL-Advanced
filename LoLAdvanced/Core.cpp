#include "StdAfx.hpp"

CCore* CCore::s_lpcCore;

CCore::CCore( HMODULE hModule )
	: m_hModule( hModule )
{
}

CCore::~CCore( void )
{
}

bool
CCore::Initialize( void )
{
	m_hWnd = FindWindow( NULL, L"League of Legends (TM) Client" );

	Game_Loop = m_cMemory.Patch<Game_Loop_t>( CMemory::Call, (DWORD) Game_Loop, (DWORD) New_Game_Loop );
	
	DebugCircleManager::GetSingleton( )->AddDebugCircle( Unit::GetPlayerToon( ), 10.0f, 0xAAAAAAAA );

	return true;
}

void
CCore::GameLoop( void )
{
	m_cAutomate.OnGameLoop( );
}