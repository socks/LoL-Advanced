#include "StdAfx.hpp"

CAutomate::CAutomate( void )
{
	m_dwLastBestTick = 0;
	m_dwLastCheck = 0;
	m_bInUse = false;
	m_cUnitHealth.clear( );
}

CAutomate::~CAutomate( void )
{
}

void
CAutomate::OnGameLoop( void )
{
	DWORD dwCurrentTickCount = GetTickCount( );

	if( dwCurrentTickCount - m_dwLastCheck >= 100 )
	{
		if( GetAsyncKeyState( VK_MENU ) & 0x8000 && dwCurrentTickCount - m_dwLastBestTick >= 450 && GetForegroundWindow( ) == CCore::s_lpcCore->m_hWnd )
		{
			Unit* lpcPlayer = *g_lpcLocalPlayer;


			if( m_bInUse == false )
			{
#ifdef _DEBUG
				OutputDebugStringA("Toggled On\n");
#endif//_DEBUG
				CCore::s_lpcCore->Print("Last Hit Bot is now <font color='#00FF00'>Activated</font>");
				m_bInUse = true;
			}


			Unit* lpcBestUnit = NULL;

			for( Unit** lpcIterator = (*g_lpcUnitManager).GetFirst( ); lpcIterator != (*g_lpcUnitManager).GetEnd( ); lpcIterator++ )
			{
				if( (*lpcIterator) == NULL || lpcPlayer == NULL )
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
			
				if( m_cUnitHealth.find( (*lpcIterator)->GetNetworkId( ) ) != m_cUnitHealth.end( ) )
				{
					float fDmgDealt = m_cUnitHealth[ (*lpcIterator)->GetNetworkId( ) ].front() - m_cUnitHealth[ (*lpcIterator)->GetNetworkId( ) ].back();

					if( (*lpcIterator)->GetHealth( ) - ( fDmgDealt / 1.5f + lpcPlayer->GetTotalDamage( ) ) <= 5.0f )
					{			
						lpcBestUnit = *lpcIterator;
						break;
					}
				}
				else if( lpcPlayer->GetTotalDamage( ) * 2.0f >= (*lpcIterator)->GetHealth( ) )
				{
					lpcBestUnit = *lpcIterator;
					break;
				}
			}
		
			if( lpcBestUnit != NULL )
			{
				m_dwLastBestTick = dwCurrentTickCount;
#ifdef _DEBUG				
				char szBuffer[128];
				sprintf(szBuffer, "%d Issuing Last Hit | Minion Current Health: %04.02f | Health in 750ms: %04.02f\n",dwCurrentTickCount,lpcBestUnit->GetHealth( ),lpcBestUnit->GetHealth( ) - ( ( m_cUnitHealth[ lpcBestUnit->GetNetworkId( ) ].front( ) - m_cUnitHealth[ lpcBestUnit->GetNetworkId( ) ].back( ) ) / 1.5f ) );
				OutputDebugStringA(szBuffer);
#endif//_DEBUG

				// Attempt to animation cancel
				float fZero[ ] = { 0.0f, 0.0f, 0.0f };
				Unit_IssueOrder( lpcPlayer, 37, fZero , 0, 0, 0, true );
				Unit_IssueOrder( lpcPlayer, 3, lpcBestUnit->GetPos( ), lpcBestUnit, 0, 0, true );
			}
		}
	
		for( Unit** lpcIterator = (*g_lpcUnitManager).GetFirst( ); lpcIterator != (*g_lpcUnitManager).GetEnd( ); lpcIterator++ )
		{
			if( (*lpcIterator) == NULL || (*g_lpcLocalPlayer) == NULL )
			{
				continue;
			}

			if( (*lpcIterator)->GetTeam( ) == (*g_lpcLocalPlayer)->GetTeam( ) )
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

			m_cUnitHealth[ (*lpcIterator)->GetNetworkId( ) ].push_back((*lpcIterator)->GetHealth( ));
			if( m_cUnitHealth[ (*lpcIterator)->GetNetworkId( ) ].size( ) > 10 )
			{
				m_cUnitHealth[ (*lpcIterator)->GetNetworkId( ) ].pop_front( );
			}
			
		}

		m_dwLastCheck = dwCurrentTickCount;
	}

	if( ! (GetAsyncKeyState( VK_MENU ) & 0x8000) )
	{
		if( m_bInUse == true )
		{
#ifdef _DEBUG
			OutputDebugStringA("Toggled Off\n");
#endif//DEBUG
			CCore::s_lpcCore->Print("Last Hit Bot is now <font color='#FF0000'>Deactivated</font>");
			m_bInUse = false;
		}
	}

}
