#include "StdAfx.hpp"

CAutomate::CAutomate( void )
{
	m_dwAttackTick = 0;
	m_dwLastCheck = 0;
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
		if( GetAsyncKeyState( VK_MENU ) & 0x8000 && GetTickCount( ) - m_dwAttackTick >= 450 && GetForegroundWindow( ) == CCore::s_lpcCore->m_hWnd )
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
			
				if( m_cUnitHealth.find( (*lpcIterator)->GetNetworkId( ) ) != m_cUnitHealth.end( ) )
				{
					float fDmgDealt = m_cUnitHealth[ (*lpcIterator)->GetNetworkId( ) ] - (*lpcIterator)->GetHealth( );

					if( (*lpcIterator)->GetHealth( ) - ( fDmgDealt / 2 + lpcPlayer->GetTotalDamage( ) ) <= 5.0f )
					{			
						lpcBestUnit = *lpcIterator;
						break;
					}
				}
				else if( lpcPlayer->GetTotalDamage( ) * 1.05f >= (*lpcIterator)->GetHealth( ) )
				{
					lpcBestUnit = *lpcIterator;
					break;
				}
			}
		
			if( bUnitWithinRange == true )
			{
				if( lpcBestUnit != NULL )
				{
					m_dwAttackTick = GetTickCount( );
				}

				Unit_IssueOrder( lpcPlayer, lpcBestUnit == NULL ? 2 : 3, lpcBestUnit == NULL ? lpcPlayer->GetPos( ) : lpcBestUnit->GetPos( ), lpcBestUnit, 0, 0, true );
			}
		}
	
		for( Unit** lpcIterator = (*g_lpcUnitManager).GetFirst( ); lpcIterator != (*g_lpcUnitManager).GetEnd( ); lpcIterator++ )
		{
			if( (*lpcIterator) == NULL )
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

			m_cUnitHealth[ (*lpcIterator)->GetNetworkId( ) ] = (*lpcIterator)->GetHealth( );
		}

		m_dwLastCheck = GetTickCount( );
	}
}