#include "StdAfx.hpp"
#include "Unit.hpp"

float
Unit::CalcDistance( Unit* lpcUnitA, Unit* lpcUnitB )
{
	float* lpfPosA = lpcUnitA->GetPos( );
	float* lpfPosB = lpcUnitB->GetPos( );

	return sqrt( pow( lpfPosA[ 0 ] - lpfPosB[ 0 ], 2 ) + pow( lpfPosA[ 1 ] - lpfPosB[ 1 ], 2 ) );
}

Unit*
Unit::GetUnitByNetworkId( DWORD dwNetworkId )
{
	return Unit_GetUnitByNetworkId( dwNetworkId );
}

Unit*
Unit::GetPlayerToon( void )
{
	return *g_lpcLocalPlayer;
}