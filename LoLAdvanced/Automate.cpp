#include "StdAfx.hpp"

CAutomate::CAutomate( void )
{
	m_dwLastBestTick = 0;
	m_dwLastAutoTick = 0;
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
	if( GetTickCount( ) - m_dwLastCheck >= 100 )
	{
		if( GetAsyncKeyState( VK_MENU ) & 0x8000 && GetTickCount( ) - m_dwLastBestTick >= 450 && GetForegroundWindow( ) == CCore::s_lpcCore->m_hWnd )
		{
			Unit* lpcPlayer = *g_lpcLocalPlayer;

			if( m_bInUse == false )
			{
				OutputDebugStringA("Toggled On\n");
				m_bInUse = true;
			}

			Unit* lpcBestUnit = NULL;
			bool bUnitWithinRange = false;

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

				bUnitWithinRange = true;
			
				if( m_cUnitHealth.find( (*lpcIterator)->GetNetworkId( ) ) != m_cUnitHealth.end( ) )
				{
					float fDmgDealt = m_cUnitHealth[ (*lpcIterator)->GetNetworkId( ) ].front() - m_cUnitHealth[ (*lpcIterator)->GetNetworkId( ) ].back();

					if( (*lpcIterator)->GetHealth( ) - ( fDmgDealt / 2 + lpcPlayer->GetTotalDamage( ) ) <= 5.0f )
					{			
						lpcBestUnit = *lpcIterator;
						break;
					}
				}
				else if( lpcPlayer->GetTotalDamage( ) * 2.f >= (*lpcIterator)->GetHealth( ) )
				{
					lpcBestUnit = *lpcIterator;
					break;
				}
			}
		
			if( bUnitWithinRange == true )
			{
				if( lpcBestUnit != NULL )
				{
					m_dwLastBestTick = GetTickCount( );
				}
				
				if( lpcBestUnit != NULL || GetTickCount( ) - m_dwLastAutoTick >= 450 )
				{
					char szBuffer[40];
					sprintf(szBuffer, "%d Issuing %s\n",GetTickCount( ),lpcBestUnit==NULL ? "Auto Attack" : "Last Hit");
					OutputDebugStringA(szBuffer);

					Unit_IssueOrder( lpcPlayer, lpcBestUnit == NULL ? 2 : 3, lpcBestUnit == NULL ? lpcPlayer->GetPos( ) : lpcBestUnit->GetPos( ), lpcBestUnit, 0, 0, true );
				}

				if( lpcBestUnit == NULL )
				{
					m_dwLastAutoTick = GetTickCount( );
				}
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

		m_dwLastCheck = GetTickCount( );
	}
	if( ! (GetAsyncKeyState( VK_MENU ) & 0x8000) )
	{
		if( m_bInUse == true )
		{
			OutputDebugStringA("Toggled Off\n");
			m_bInUse = false;
		}
	}
}