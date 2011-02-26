#include "StdAfx.hpp"

CCore* CCore::s_lpcCore;

CCore::CCore( HMODULE hModule )
	: m_hModule( hModule )
{
	m_dwLastCheck = 0;
}

CCore::~CCore( void )
{
}

bool
CCore::Initialize( void )
{
	m_hWnd = FindWindow( NULL, L"League of Legends (TM) Client" );
//	Game_ProcessWorldEvent2 = m_cMemory.Patch<DWORD>( CMemory::Inline, (DWORD) Game_ProcessWorldEvent2, (DWORD) New_Game_ProcessWorldEvent2 );
//	NetApp_ClientFacade_SendPacket = m_cMemory.Patch<DWORD>( CMemory::VMT, (DWORD) NetApp_ClientFacade_SendPacket, (DWORD) New_NetApp_ClientFacade_SendPacket );
//	DebugBreak( );
	Game_Loop = m_cMemory.Patch<Game_Loop_t>( CMemory::Call, (DWORD) Game_Loop, (DWORD) New_Game_Loop );

	MessageBox( NULL, L"LoL!Advanced by rolle3k\nalways check www.Skillhackers.info for updates\n\nif you paid for this you got screwed!", L"LoL!Advanced l0aded", MB_OK );

	return true;
}

/*
bool
CCore::BeforePacketReceived( BYTE yType, BYTE* lpyPacket, DWORD dwLength )
{
	return true;
}*/

void
CCore::GameLoop( void )
{
	if( GetAsyncKeyState( VK_MENU ) & 0x8000 && GetTickCount( ) - m_dwLastCheck > 100 && GetForegroundWindow( ) == m_hWnd )
	{
		Unit* lpcPlayer = *g_lpcLocalPlayer;

		Unit* lpcBestUnit = NULL;
		bool bUnitWithinRange = false;

		for( Unit** lpcIterator = (*g_lpcUnitManager).GetFirst( ); lpcIterator != (*g_lpcUnitManager).GetEnd( ); lpcIterator++ )
		{
			if( (*lpcIterator) == NULL )
			{
				continue;
			}

			if( (*lpcIterator)->GetTeam( ) == lpcPlayer->GetTeam( ) )
			{
				continue;
			}

			if( (*lpcIterator)->IsDead( ) == true )
			{
				continue;
			}

			if( memcmp( (*lpcIterator)->GetName( ), "Minion_", 7 ) )
			{
				continue;
			}

			if( Unit::CalcDistance( lpcPlayer, *lpcIterator ) > lpcPlayer->GetRange( ) )
			{
				continue;
			}

			bUnitWithinRange = true;

			if( lpcPlayer->GetTotalDamage( ) * 1.10f >= (*lpcIterator)->GetHealth( ) )
			{
				lpcBestUnit = *lpcIterator;
				break;
			}
		}
		
		if( bUnitWithinRange == true )
		{
			Unit_IssueOrder( lpcPlayer, lpcBestUnit == NULL ? 2 : 3, lpcBestUnit == NULL ? lpcPlayer->GetPos( ) : lpcBestUnit->GetPos( ), lpcBestUnit, 0, 0, true );
		}

		m_dwLastCheck = GetTickCount( );
	}
}